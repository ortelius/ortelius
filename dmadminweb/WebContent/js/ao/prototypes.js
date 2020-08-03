/*
 *
 *  Ortelius for Microservice Configuration Mapping
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
