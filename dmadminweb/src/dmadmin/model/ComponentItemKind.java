package dmadmin.model;

public enum ComponentItemKind {
 DATABASE(0),
 FILE(1),
 DOCKER(2);
 
 private int m_value;
 
 private ComponentItemKind(int value)  { m_value = value; }
 
 public int value()  { return m_value; }
 
 public static ComponentItemKind fromInt(int value) {
  for(ComponentItemKind k : values()) {
   if(k.value() == value) {
    return k;
   }
  }
  return FILE;
 }
}