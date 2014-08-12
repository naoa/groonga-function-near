/* 
  Copyright(C) 2014 Naoya Murakami <naoya@createfield.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <groonga/plugin.h>

#ifdef __GNUC__
# define GNUC_UNUSED __attribute__((__unused__))
#else
# define GNUC_UNUSED
#endif

static grn_obj *
func_near(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args, grn_user_data *user_data)
{
  grn_obj *found;

  found = grn_plugin_proc_alloc(ctx, user_data, GRN_DB_BOOL, 0);
  if (!found) {
    return NULL;
  }
  GRN_BOOL_SET(ctx, found, GRN_TRUE);

  return found;
}

static grn_rc
selector_near(grn_ctx *ctx, GNUC_UNUSED grn_obj *table, GNUC_UNUSED grn_obj *index,
              GNUC_UNUSED int nargs, grn_obj **args,
              grn_obj *res, grn_operator op)
{
  grn_rc rc = GRN_SUCCESS;
  grn_obj *index_column;
  unsigned int n_indexes;

  grn_obj *column = args[1];
  grn_obj *max_interval = args[2];
  grn_obj *keywords_string = args[3];

  n_indexes = grn_column_index(ctx, column, GRN_OP_MATCH, &index_column, 1, NULL);
  if (n_indexes) {
    grn_search_optarg options;
    options.mode = GRN_OP_NEAR;
    options.similarity_threshold = 0;
    options.max_interval = GRN_UINT32_VALUE(max_interval);
    options.weight_vector = NULL;
    options.vector_size = 1;
    options.proc = NULL;
    options.max_size = 0;
    grn_obj_search(ctx, index_column, keywords_string, res, op, &options);
  } else {
    return GRN_INVALID_ARGUMENT;
  }

  return rc;
}

grn_rc
GRN_PLUGIN_INIT(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  {
    grn_obj *selector_proc;

    selector_proc = grn_proc_create(ctx, "near", -1, GRN_PROC_FUNCTION,
                                    func_near, NULL, NULL, 0, NULL);
    grn_proc_set_selector(ctx, selector_proc, selector_near);
  }

  return ctx->rc;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
