/*****************************************************************************
 * cmdline.c: command line parsing
 *****************************************************************************
 * Copyright (C) 2001-2007 VLC authors and VideoLAN
 * $Id$
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include "../libvlc.h"
#include <vlc_actions.h>
#include <vlc_charset.h>
#include <vlc_modules.h>
#include <vlc_plugin.h>

#include "vlc_getopt.h"

#include "configuration.h"
#include "modules/modules.h"

#include <assert.h>

static void ListChoiceHelp( vlc_object_t *p_this, const char *psz_name,
                            const char *psz_desc )
{
    module_config_t *cfg = config_FindConfig( psz_name );
    bool is_module = cfg && ( cfg->i_type == CONFIG_ITEM_MODULE ||
                              cfg->i_type == CONFIG_ITEM_MODULE_CAT ||
                              cfg->i_type == CONFIG_ITEM_MODULE_LIST ||
                              cfg->i_type == CONFIG_ITEM_MODULE_LIST_CAT );

    if( is_module )
    {
        /* For module options, list using user-friendly shortcuts (last
         * shortcut is usually the nice short name the user types after -V).
         * This avoids internal names like "vdpau_display" and duplicates
         * from submodules. */
        const char *cap = cfg->psz_type;
        module_t **mods = NULL;
        ssize_t n = 0;

        if( cap != NULL )
            n = module_list_cap( &mods, cap );

        printf( "Available %s:\n", ( psz_desc && *psz_desc ) ? psz_desc : psz_name );

        /* any */
        printf( "  %-20s %s\n", "any", "Automatic" );

        if( n > 0 && mods != NULL )
        {
            for( ssize_t i = 0; i < n; i++ )
            {
                module_t *m = mods[i];
                const char *name = (m->i_shortcuts > 0)
                    ? m->pp_shortcuts[ m->i_shortcuts - 1 ]
                    : module_get_object(m);
                const char *text = module_gettext( m, module_get_name(m, true) );
                printf( "  %-20s %s\n", name, text ? text : "" );
            }
        }

        /* none */
        printf( "  %-20s %s\n", "none", "Disable" );

        module_list_free( mods );
        exit( 0 );
    }

    /* Fallback for other string choices */
    char **ppsz_values, **ppsz_texts;
    ssize_t i_count = config_GetPszChoices( p_this, psz_name,
                                            &ppsz_values, &ppsz_texts );
    if( i_count > 0 )
    {
        printf( "Available %s:\n", ( psz_desc && *psz_desc ) ? psz_desc : psz_name );
        for( ssize_t i = 0; i < i_count; i++ )
            printf( "  %-20s %s\n", ppsz_values[i],
                    ( ppsz_texts[i] && *ppsz_texts[i] ) ? ppsz_texts[i] : "" );

        for( ssize_t i = 0; i < i_count; i++ )
        {
            free( ppsz_values[i] );
            free( ppsz_texts[i] );
        }
        free( ppsz_values );
        free( ppsz_texts );
    }
    exit( 0 );
}

#undef config_LoadCmdLine
/**
 * Parse command line for configuration options.
 *
 * Now that the module_bank has been initialized, we can dynamically
 * generate the longopts structure used by getops. We have to do it this way
 * because we don't know (and don't want to know) in advance the configuration
 * options used (ie. exported) by each module.
 *
 * @param p_this object to write command line options as variables to
 * @param i_argc number of command line arguments
 * @param ppsz_args commandl ine arguments [IN/OUT]
 * @param pindex NULL to ignore unknown options,
 *               otherwise index of the first non-option argument [OUT]
 * @return 0 on success, -1 on error.
 */
int config_LoadCmdLine( vlc_object_t *p_this, int i_argc,
                        const char *ppsz_argv[], int *pindex )
{
    int i_cmd, i_index, i_opts, i_shortopts, flag, i_verbose = 0;
    struct vlc_option *p_longopts;
    const char **argv_copy = NULL;
#define b_ignore_errors (pindex == NULL)

    /* Short options */
    const module_config_t *pp_shortopts[256];
    char *psz_shortopts;

    /*
     * Generate the longopts and shortopts structures used by getopt_long
     */

    i_opts = 0;
    for (const vlc_plugin_t *p = vlc_plugins; p != NULL; p = p->next)
        /* count the number of exported configuration options (to allocate
         * longopts). We also need to allocate space for two options when
         * dealing with boolean to allow for --foo and --no-foo */
        i_opts += p->conf.count + 2 * p->conf.booleans;

    p_longopts = vlc_alloc( i_opts + 1, sizeof(*p_longopts)  );
    if( p_longopts == NULL )
        return -1;

    psz_shortopts = malloc( 2 * i_opts + 1 );
    if( psz_shortopts == NULL )
    {
        free( p_longopts );
        return -1;
    }

    /* If we are requested to ignore errors, then we must work on a copy
     * of the ppsz_argv array, otherwise getopt_long will reorder it for
     * us, ignoring the arity of the options */
    if( b_ignore_errors )
    {
        argv_copy = vlc_alloc( i_argc, sizeof(char *) );
        if( argv_copy == NULL )
        {
            free( psz_shortopts );
            free( p_longopts );
            return -1;
        }
        memcpy( argv_copy, ppsz_argv, i_argc * sizeof(char *) );
        ppsz_argv = argv_copy;
    }

    i_shortopts = 0;
    for( i_index = 0; i_index < 256; i_index++ )
    {
        pp_shortopts[i_index] = NULL;
    }

    /* Fill the p_longopts and psz_shortopts structures */
    i_index = 0;
    for (const vlc_plugin_t *p = vlc_plugins; p != NULL; p = p->next)
    {
        for (const module_config_t *p_item = p->conf.items,
                                   *p_end = p_item + p->conf.size;
             p_item < p_end;
             p_item++)
        {
            /* Ignore hints */
            if( !CONFIG_ITEM(p_item->i_type) )
                continue;

            /* Add item to long options */
            p_longopts[i_index].name = strdup( p_item->psz_name );
            if( p_longopts[i_index].name == NULL ) continue;
            p_longopts[i_index].flag = &flag;
            p_longopts[i_index].val = 0;

            if( CONFIG_CLASS(p_item->i_type) != CONFIG_ITEM_BOOL )
                p_longopts[i_index].has_arg = true;
            else
            /* Booleans also need --no-foo and --nofoo options */
            {
                char *psz_name;

                p_longopts[i_index].has_arg = false;
                i_index++;

                if( asprintf( &psz_name, "no%s", p_item->psz_name ) == -1 )
                    continue;
                p_longopts[i_index].name = psz_name;
                p_longopts[i_index].has_arg = false;
                p_longopts[i_index].flag = &flag;
                p_longopts[i_index].val = 1;
                i_index++;

                if( asprintf( &psz_name, "no-%s", p_item->psz_name ) == -1 )
                    continue;
                p_longopts[i_index].name = psz_name;
                p_longopts[i_index].has_arg = false;
                p_longopts[i_index].flag = &flag;
                p_longopts[i_index].val = 1;
            }
            i_index++;

            /* If item also has a short option, add it */
            if( p_item->i_short )
            {
                pp_shortopts[(int)p_item->i_short] = p_item;
                psz_shortopts[i_shortopts] = p_item->i_short;
                i_shortopts++;
                if( p_item->i_type != CONFIG_ITEM_BOOL
                 && p_item->i_short != 'v' )
                {
                    psz_shortopts[i_shortopts] = ':';
                    i_shortopts++;
                }
            }
        }
    }

    /* Close the longopts and shortopts structures */
    memset( &p_longopts[i_index], 0, sizeof(*p_longopts) );
    psz_shortopts[i_shortopts] = '\0';

    int ret = -1;

    /*
     * Parse the command line options
     */
    vlc_getopt_t state;
    state.ind = 0 ; /* set to 0 to tell GNU getopt to reinitialize */
    while( ( i_cmd = vlc_getopt_long( i_argc, (char **)ppsz_argv,
                                      psz_shortopts,
                                      p_longopts, &i_index, &state ) ) != -1 )
    {
        /* A long option has been recognized */
        if( i_cmd == 0 )
        {
            module_config_t *p_conf;
            const char *psz_name = p_longopts[i_index].name;

            /* Check if we deal with a --nofoo or --no-foo long option */
            if( flag ) psz_name += psz_name[2] == '-' ? 3 : 2;

            /* Store the configuration option */
            p_conf = config_FindConfig( psz_name );
            if( p_conf )
            {
                /* Check if the option is deprecated */
                if( p_conf->b_removed )
                {
                    fprintf(stderr,
                            "Warning: option --%s no longer exists.\n",
                            psz_name);
                    continue;
                }

                if( pindex != NULL && state.arg )
                {
                    if( !strcasecmp( state.arg, "help" ) ||
                        (state.arg[0] == '-' && state.arg[1] != '\0') )
                    {
                        if( p_conf->i_type == CONFIG_ITEM_MODULE ||
                            p_conf->i_type == CONFIG_ITEM_MODULE_CAT ||
                            p_conf->i_type == CONFIG_ITEM_MODULE_LIST ||
                            p_conf->i_type == CONFIG_ITEM_MODULE_LIST_CAT )
                        {
                            ListChoiceHelp( p_this, psz_name, p_conf->psz_text );
                        }
                    }
                    else if( strcasecmp( state.arg, "any" ) &&
                             strcasecmp( state.arg, "none" ) &&
                             (p_conf->i_type == CONFIG_ITEM_MODULE ||
                              p_conf->i_type == CONFIG_ITEM_MODULE_CAT ||
                              p_conf->i_type == CONFIG_ITEM_MODULE_LIST ||
                              p_conf->i_type == CONFIG_ITEM_MODULE_LIST_CAT) )
                    {
                        /* Validate that the value is a known module for this option */
                        if( p_conf->psz_type != NULL )
                        {
                            bool valid = false;
                            module_t **mods = NULL;
                            ssize_t n = module_list_cap( &mods, p_conf->psz_type );
                            if( n > 0 && mods != NULL )
                            {
                                for( ssize_t i = 0; i < n && !valid; i++ )
                                {
                                    for( unsigned s = 0; s < mods[i]->i_shortcuts; s++ )
                                    {
                                        if( !strcasecmp( mods[i]->pp_shortcuts[s], state.arg ) )
                                        {
                                            valid = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            module_list_free( mods );
                            if( n > 0 && !valid )
                            {
                                fprintf( stderr,
                                         "unknown %s module: %s\n"
                                         "Use \"%s help\" to list available modules.\n",
                                         psz_name, state.arg, psz_name );
                                exit( 1 );
                            }
                        }
                    }
                }

                switch( CONFIG_CLASS(p_conf->i_type) )
                {
                    case CONFIG_ITEM_STRING:
                        var_Create( p_this, psz_name, VLC_VAR_STRING );
                        var_SetString( p_this, psz_name, state.arg );
                        break;
                    case CONFIG_ITEM_INTEGER:
                        var_Create( p_this, psz_name, VLC_VAR_INTEGER );
                        var_Change( p_this, psz_name, VLC_VAR_SETMINMAX,
                            &(vlc_value_t){ .i_int = p_conf->min.i },
                            &(vlc_value_t){ .i_int = p_conf->max.i } );
                        var_SetInteger( p_this, psz_name,
                                        strtoll(state.arg, NULL, 0));
                        break;
                    case CONFIG_ITEM_FLOAT:
                        var_Create( p_this, psz_name, VLC_VAR_FLOAT );
                        var_Change( p_this, psz_name, VLC_VAR_SETMINMAX,
                            &(vlc_value_t){ .f_float = p_conf->min.f },
                            &(vlc_value_t){ .f_float = p_conf->max.f } );
                        var_SetFloat( p_this, psz_name, us_atof(state.arg) );
                        break;
                    case CONFIG_ITEM_BOOL:
                        var_Create( p_this, psz_name, VLC_VAR_BOOL );
                        var_SetBool( p_this, psz_name, !flag );
                        break;
                }
                continue;
            }
        }

        /* A short option has been recognized */
        if( pp_shortopts[i_cmd] != NULL )
        {
            const char *name = pp_shortopts[i_cmd]->psz_name;
            const module_config_t *p_item = pp_shortopts[i_cmd];

            if( pindex != NULL && state.arg )
            {
                if( !strcasecmp( state.arg, "help" ) ||
                    (state.arg[0] == '-' && state.arg[1] != '\0') )
                {
                    if( p_item->i_type == CONFIG_ITEM_MODULE ||
                        p_item->i_type == CONFIG_ITEM_MODULE_CAT ||
                        p_item->i_type == CONFIG_ITEM_MODULE_LIST ||
                        p_item->i_type == CONFIG_ITEM_MODULE_LIST_CAT )
                    {
                        ListChoiceHelp( p_this, name, p_item->psz_text );
                    }
                }
                else if( strcasecmp( state.arg, "any" ) &&
                         strcasecmp( state.arg, "none" ) &&
                         (p_item->i_type == CONFIG_ITEM_MODULE ||
                          p_item->i_type == CONFIG_ITEM_MODULE_CAT ||
                          p_item->i_type == CONFIG_ITEM_MODULE_LIST ||
                          p_item->i_type == CONFIG_ITEM_MODULE_LIST_CAT) )
                {
                    if( p_item->psz_type != NULL )
                    {
                        bool valid = false;
                        module_t **mods = NULL;
                        ssize_t n = module_list_cap( &mods, p_item->psz_type );
                        if( n > 0 && mods != NULL )
                        {
                            for( ssize_t i = 0; i < n && !valid; i++ )
                            {
                                for( unsigned s = 0; s < mods[i]->i_shortcuts; s++ )
                                {
                                    if( !strcasecmp( mods[i]->pp_shortcuts[s], state.arg ) )
                                    {
                                        valid = true;
                                        break;
                                    }
                                }
                            }
                        }
                        module_list_free( mods );
                        if( n > 0 && !valid )
                        {
                            fprintf( stderr,
                                     "unknown %s module: %s\n"
                                     "Use \"%s help\" to list available modules.\n",
                                     name, state.arg, name );
                            exit( 1 );
                        }
                    }
                }
            }

            switch( CONFIG_CLASS(p_item->i_type) )
            {
                case CONFIG_ITEM_STRING:
                    var_Create( p_this, name, VLC_VAR_STRING );
                    var_SetString( p_this, name, state.arg );
                    break;
                case CONFIG_ITEM_INTEGER:
                    var_Create( p_this, name, VLC_VAR_INTEGER );
                    if( i_cmd == 'v' )
                    {
                        i_verbose++; /* -v */
                        var_SetInteger( p_this, name, i_verbose );
                    }
                    else
                    {
                        var_SetInteger( p_this, name,
                                        strtoll(state.arg, NULL, 0) );
                    }
                    break;
                case CONFIG_ITEM_BOOL:
                    var_Create( p_this, name, VLC_VAR_BOOL );
                    var_SetBool( p_this, name, true );
                    break;
            }

            continue;
        }

        /* Internal error: unknown option */
        if( !b_ignore_errors )
        {
            const module_config_t *bad_mod = NULL;

            if( state.opt )
            {
                bad_mod = pp_shortopts[(unsigned char)state.opt];
                if (bad_mod && (bad_mod->i_type == CONFIG_ITEM_MODULE ||
                                bad_mod->i_type == CONFIG_ITEM_MODULE_CAT ||
                                bad_mod->i_type == CONFIG_ITEM_MODULE_LIST ||
                                bad_mod->i_type == CONFIG_ITEM_MODULE_LIST_CAT))
                {
                    ListChoiceHelp(p_this, bad_mod->psz_name, bad_mod->psz_text);
                }
                fputs( "vlc: unknown option or missing mandatory argument `", stderr );
                fprintf( stderr, "-%c'\n", state.opt );
            }
            else
            {
                const char *tok = ppsz_argv[state.ind-1];

                if (strncmp(tok, "--", 2) == 0) {
                    const char *nm = tok + 2;
                    char *eq = strchr(nm, '=');
                    if (eq) *eq = '\0';
                    bad_mod = config_FindConfig(nm);
                    if (eq) *eq = '=';
                    if (bad_mod && (bad_mod->i_type == CONFIG_ITEM_MODULE ||
                                    bad_mod->i_type == CONFIG_ITEM_MODULE_CAT ||
                                    bad_mod->i_type == CONFIG_ITEM_MODULE_LIST ||
                                    bad_mod->i_type == CONFIG_ITEM_MODULE_LIST_CAT))
                    {
                        ListChoiceHelp(p_this, bad_mod->psz_name, bad_mod->psz_text);
                    }
                }

                fputs( "vlc: unknown option or missing mandatory argument `", stderr );
                fprintf( stderr, "%s'\n", tok );
            }

            fputs( "Try `vlc --help' for more information.\n", stderr );
            goto out;
        }
    }

    ret = 0;
    if( pindex != NULL )
        *pindex = state.ind;
out:
    /* Free allocated resources */
    for( i_index = 0; p_longopts[i_index].name; i_index++ )
        free( (char *)p_longopts[i_index].name );
    free( p_longopts );
    free( psz_shortopts );
    free( argv_copy );
    return ret;
}

