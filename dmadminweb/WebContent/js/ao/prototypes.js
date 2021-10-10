/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

String.prototype.replaceAll = function(search, replace)
{
    //if replace is null, return original string otherwise it will
    //replace search string with 'undefined'.
    if(!replace) 
        return this;

    return this.replace(new RegExp('[' + search + ']', 'g'), replace);
};

String.prototype.trimLeft = function(charlist) {
 if (charlist === undefined)
   charlist = "\s";

 return this.replace(new RegExp("^[" + charlist + "]+"), "");
};

String.prototype.trimRight = function(charlist) {
 if (charlist === undefined)
   charlist = "\s";

 return this.replace(new RegExp("[" + charlist + "]+$"), "");
};

String.prototype.trimChars = function(charlist) {
 return this.trimLeft(charlist).trimRight(charlist);
};
