package dmadmin.model;

import javax.naming.ldap.Control;

class FastBindConnectionControl implements Control {
 /**
  * 
  */
 private static final long serialVersionUID = -2243987315165749497L;

 public byte[] getEncodedValue() {
     return null;
 }
 
 public String getID() {
       return "1.2.840.113556.1.4.1781";
 }
 
 public boolean isCritical() {
        return true;
  }
 }
                                                      
