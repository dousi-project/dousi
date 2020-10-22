/* DO NOT EDIT THIS FILE - it is machine generated */

#include "org_dousi_client_NativeRpcClient.h"

#include "core/dousi.h"
#include "native_java_client.h"


#include <iostream>

#include <jni.h>

JavaVM *jvm;

jclass java_class_NativeRpcClient;
jmethodID java_method_onResultReceived;
jobject java_object_native_rpc_client;

inline jclass LoadClass(JNIEnv *env, const char *class_name) {
    jclass tempLocalClassRef = env->FindClass(class_name);
    jclass ret = (jclass)env->NewGlobalRef(tempLocalClassRef);
//    assert(ret);
    env->DeleteLocalRef(tempLocalClassRef);
    return ret;
}

/// Load and cache frequently-used Java classes and methods
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), 0x00010008) != JNI_OK) {
        return JNI_ERR;
    }
    jvm = vm;
    java_class_NativeRpcClient = LoadClass(env, "org/dousi/client/NativeRpcClient");
    java_method_onResultReceived = env->GetMethodID(java_class_NativeRpcClient, "onReturnValueReceived", "(J[B)V");
    return 0x00010008;
}

//void JNI_OnUnload(JavaVM *vm, void *reserved) {}

inline std::string JavaStringToNativeString(JNIEnv *env, jstring jstr) {
    const char *c_str = env->GetStringUTFChars(jstr, nullptr);
    std::string result(c_str);
    env->ReleaseStringUTFChars(static_cast<jstring>(jstr), c_str);
    return result;
}

/// Convert C++ String to a Java ByteArray.
inline jbyteArray NativeStringToJavaByteArray(JNIEnv *env, const std::string &str) {
    jbyteArray array = env->NewByteArray(str.size());
    env->SetByteArrayRegion(array, 0, str.size(),
                            reinterpret_cast<const jbyte *>(str.c_str()));
    return array;
}

inline std::string JavaByteArrayToNativeString(JNIEnv *env, const jbyteArray &bytes) {
    const auto size = env->GetArrayLength(bytes);
    std::string str(size, 0);
    env->GetByteArrayRegion(bytes, 0, size, reinterpret_cast<jbyte *>(&str.front()));
    return str;
}

// TODO(qwang)
// JavaByteArrayToSBuffer


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     org_restrpc_client_NativeRpcClient
 * Method:    nativeNewRpcClient
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_dousi_client_NativeRpcClient_nativeNewRpcClient
        (JNIEnv *env, jobject o, jstring serverAddress) {

    java_object_native_rpc_client = (jobject) env->NewGlobalRef(o);

    auto on_return_value_received = [](long request_id, const std::shared_ptr<char> &buffer_ptr, const size_t buffer_size) {
        JNIEnv *env = nullptr;
        jvm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
        jbyteArray javaByteArray = NativeStringToJavaByteArray(env, std::string(buffer_ptr.get(), buffer_size));
        env->CallVoidMethod(java_object_native_rpc_client, java_method_onResultReceived, request_id, javaByteArray);
    };

    const std::string server_addr = JavaStringToNativeString(env, serverAddress);
    auto *native_rpc_client = new dousi::NativeClientForJava {on_return_value_received, server_addr};
    dousi::DousiLog::StartDousiLog("tmp/dousi/java-client.log",
                                   dousi::LOG_LEVEL::DEBUG, 10, 3);
    return reinterpret_cast<long>(native_rpc_client);
}

/*
 * Class:     org_dousi_client_NativeRpcClient
 * Method:    nativeInvoke
 * Signature: (J[B)J
 */
JNIEXPORT jlong JNICALL Java_org_dousi_client_NativeRpcClient_nativeInvoke
        (JNIEnv *env, jobject o, jlong rpcClientPointer, jbyteArray encodedBytes) {
    auto *native_rpc_client = reinterpret_cast<dousi::NativeClientForJava *>(rpcClientPointer);
    auto encodedFuncNameAndArgs = JavaByteArrayToNativeString(env, encodedBytes);
    return native_rpc_client->Invoke(encodedFuncNameAndArgs);
}

/*
 * Class:     org_dousi_client_NativeRpcClient
 * Method:    nativeDestroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_dousi_client_NativeRpcClient_nativeDestroy
(JNIEnv *env, jobject o, jlong rpcClientPointer) {
    auto *native_rpc_client = reinterpret_cast<dousi::NativeClientForJava *>(rpcClientPointer);
    native_rpc_client->Close();
    delete native_rpc_client;

    env->DeleteGlobalRef(java_class_NativeRpcClient);
    env->DeleteGlobalRef(reinterpret_cast<jobject>(java_method_onResultReceived));
    env->DeleteGlobalRef(java_object_native_rpc_client);
}

#ifdef __cplusplus
}
#endif