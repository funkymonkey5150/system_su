/*
** Copyright 2010, Adam Shanks (@ChainsDD)
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "su.h"

int database_check(const struct su_context *ctx)
{
    FILE *fp;
    char filename[PATH_MAX];
    char allow[7];
    int last = 0;

    snprintf(filename, sizeof(filename),
                REQUESTOR_STORED_PATH "/%u-%u", ctx->from.uid, ctx->to.uid);
    if ((fp = fopen(filename, "r"))) {
        LOGD("Found file %s", filename);
        
        fgets(allow, sizeof(allow), fp);
        last = strlen(allow) - 1;
        if (last >= 0)
        	allow[last] = 0;
        	
        char cmd[ARG_MAX];
        fgets(cmd, sizeof(cmd), fp);
        /* skip trailing '\n' */
        last = strlen(cmd) - 1;
        if (last >= 0)
            cmd[last] = 0;

        LOGD("Comparing '%s' to '%s'", cmd, get_command(&ctx->to));
        if (strcmp(cmd, get_command(&ctx->to)) != 0) {
            strcpy(allow, "prompt");
        }
        fclose(fp);
    } else if ((fp = fopen(REQUESTOR_STORED_DEFAULT, "r"))) {
        LOGD("Using default file %s", REQUESTOR_STORED_DEFAULT);
        fgets(allow, sizeof(allow), fp);
        last = strlen(allow) - 1;
        if (last >=0)
            allow[last] = 0;
        
        fclose(fp);
    }

    if (strcmp(allow, "allow") == 0) {
        return ALLOW;
    } else if (strcmp(allow, "deny") == 0) {
        return DENY;
    } else {
        return INTERACTIVE;
    }
}
