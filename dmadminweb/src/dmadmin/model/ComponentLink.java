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

package dmadmin.model;

import javax.servlet.http.HttpServlet;

/**
 * Servlet implementation class ComponentLink
 */
public class ComponentLink
{
    private int m_appid;
    private int m_objfrom;
    private int m_objto;
    /**
     * @see HttpServlet#HttpServlet()
     */
    public ComponentLink() {
        super();
        // TODO Auto-generated constructor stub
    }

    public void setObjFrom(int cf) { m_objfrom = cf; }
    public void setObjTo(int ct) { m_objto = ct; }
    public void setAppId(int appid) { m_appid = appid; }

    public int getCompFrom() { return m_objfrom; }
    public int getCompTo() { return m_objto; }
    public int getAppId() { return m_appid; }
}
