/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package djondb;

public class ConnectionReference {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public ConnectionReference(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(ConnectionReference obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        djonwrapperJNI.delete_ConnectionReference(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void set_connection(DjondbConnection value) {
    djonwrapperJNI.ConnectionReference__connection_set(swigCPtr, this, DjondbConnection.getCPtr(value), value);
  }

  public DjondbConnection get_connection() {
    long cPtr = djonwrapperJNI.ConnectionReference__connection_get(swigCPtr, this);
    return (cPtr == 0) ? null : new DjondbConnection(cPtr, false);
  }

  public void set_references(int value) {
    djonwrapperJNI.ConnectionReference__references_set(swigCPtr, this, value);
  }

  public int get_references() {
    return djonwrapperJNI.ConnectionReference__references_get(swigCPtr, this);
  }

  public ConnectionReference() {
    this(djonwrapperJNI.new_ConnectionReference(), true);
  }

}
