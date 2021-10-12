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

package dmadmin.util;

import java.net.MalformedURLException;
import java.net.URL;

public final class JdbcUrl
{

 private JdbcUrl()
 {
 }

 /**
  * @return the database name from the connect string, which is typically the 'path' component, or null if we can't.
  */
 public static String getDatabaseName(String connectString)
 {
  try
  {
   String sanitizedString = null;
   int schemeEndOffset = connectString.indexOf("://");
   if (-1 == schemeEndOffset)
   {
    // couldn't find one? try our best here.
    sanitizedString = "http://" + connectString;
    System.out.println("Could not find database access scheme in connect string " + connectString);
   }
   else
   {
    sanitizedString = "http" + connectString.substring(schemeEndOffset);
   }

   URL connectUrl = new URL(sanitizedString);
   String databaseName = connectUrl.getPath();
   if (null == databaseName)
   {
    return null;
   }

   // This is taken from a 'path' part of a URL, which may have leading '/'
   // characters; trim them off.
   while (databaseName.startsWith("/"))
   {
    databaseName = databaseName.substring(1);
   }

   return databaseName;
  }
  catch (MalformedURLException mue)
  {
   System.out.println("Malformed connect string URL: " + connectString + "; reason is " + mue.toString());
   return null;
  }
 }

 /**
  * @return the hostname from the connect string, or null if we can't.
  */
 public static String getHostName(String connectString)
 {
  try
  {
   String sanitizedString = null;
   int schemeEndOffset = connectString.indexOf("://");
   if (-1 == schemeEndOffset)
   {
    // Couldn't find one? ok, then there's no problem, it should work as a
    // URL.
    sanitizedString = connectString;
   }
   else
   {
    sanitizedString = "http" + connectString.substring(schemeEndOffset);
   }

   URL connectUrl = new URL(sanitizedString);
   return connectUrl.getHost();
  }
  catch (MalformedURLException mue)
  {
   System.out.println("Malformed connect string URL: " + connectString + "; reason is " + mue.toString());
   return null;
  }
 }

 /**
  * @return the port from the connect string, or -1 if we can't.
  */
 public static int getPort(String connectString)
 {
  try
  {
   String sanitizedString = null;
   int schemeEndOffset = connectString.indexOf("://");
   if (-1 == schemeEndOffset)
   {
    // Couldn't find one? ok, then there's no problem, it should work as a
    // URL.
    sanitizedString = connectString;
   }
   else
   {
    sanitizedString = "http" + connectString.substring(schemeEndOffset);
   }

   URL connectUrl = new URL(sanitizedString);
   return connectUrl.getPort();
  }
  catch (MalformedURLException mue)
  {
   System.out.println("Malformed connect string URL: " + connectString + "; reason is " + mue.toString());
   return -1;
  }
 }

}
