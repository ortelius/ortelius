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
 */package dmadmin.pro;

import java.util.TimerTask;

import javax.servlet.ServletContext;

public class TimedTask extends TimerTask
{

 private ServletContext m_ServletContext;

 public TimedTask(ServletContext sc)
 {
  // Constructor
  m_ServletContext = sc;
 }

 public void run()
 {
  try (DMSessionPro m_session = new DMSessionPro(m_ServletContext))
  {
   m_session.syncDefects(m_ServletContext);
  }
 }
}
