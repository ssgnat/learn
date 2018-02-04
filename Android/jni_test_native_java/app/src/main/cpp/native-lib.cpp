#include <jni.h>
#include <string>
#include<android/log.h>
#include <string.h>
#include <pthread.h>

jint m_intparam = 0;
jchar m_charparam = 0;
std::string m_strparam;


#ifndef TSS_LOG
#define TSS_LOG
#define TAG "TSS_LOG"

#define TLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define TLOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define TLOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define TLOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGW类型
#endif




extern "C"
JNIEXPORT jstring
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_setStringProperties(
        JNIEnv *env,
        jobject jo/* this */)
{
    jclass classz = env->FindClass("qz/gl/gx/test_native_java");
    return env->NewStringUTF(m_strparam.c_str());
}


extern "C"
JNIEXPORT jstring
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_returnStringToJava(
        JNIEnv *env,
        jobject /* this */) {

    return env->NewStringUTF(m_strparam.c_str());
//    char* kk = new char[10];
//    strcpy(kk,"hello");
//
//    jstring js = env->NewStringUTF(kk);
//    delete[] kk;
//    kk = NULL;
//
//    return  js;
}

//
//
//extern "C"
//JNIEXPORT jstring
//JNICALL
//Java_qz_gl_gx_test_1native_1java_MainActivity_returnStringToJava(
//        JNIEnv *env,
//        jobject /* this */) {
//
////    return env->NewStringUTF(m_strparam.c_str());
//    char* kk = new char[10];
//    strcpy(kk,"hello");
//
//    jstring js = env->NewStringUTF(kk);
//    delete[] kk;
//    kk = NULL;
//
//    return  js;
//}

extern "C"
JNIEXPORT void
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_setStringIntoJni(
        JNIEnv *env,
        jobject /* this */,
        jstring strParam)
{
    jboolean isCopied;
    const char* c = env->GetStringUTFChars(strParam,&isCopied);
    if(c == NULL || !isCopied)
    {
        TLOGE("Function GetStringUTFChars ERROR!");
        return;
    }

    TLOGD("GetStringUTFChars vaiable:%s", c);
//    m_strparam.append(c);
    m_strparam = c;
    delete[] c;
    c = NULL;
}


extern "C"
JNIEXPORT void
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_setIntPara(
        JNIEnv *env,
        jobject me,
        jint intparam) {
    m_intparam = intparam;
}

extern "C"
JNIEXPORT void
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_setCharPara(
        JNIEnv *env,
        jobject me,
        jchar charparam) {
    m_charparam = charparam;
}

extern "C"
JNIEXPORT jint
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_getIntPara(
        JNIEnv *env,
        jobject me) {
    return m_intparam;
}

extern "C"
JNIEXPORT jchar
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_getCharPara(
        JNIEnv *env,
        jobject me) {
    return m_charparam;
}




extern "C"
JNIEXPORT jstring
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_getJNINewString(
                JNIEnv* env,
                jobject me
        )
{
    char* pstr = new char[100];
    strcpy(pstr, "我是从JNI返回字符串:MainActivity_getJNINewString");
    jstring js = env->NewStringUTF(pstr);
    delete[] pstr;
    pstr = NULL;
    return js;
}



//测试byte jni从网络中获取图片，然后传到java层显示；
//涉及：
//1、jni开线程从网络获取资源
//2、把数据传输到java层 - public void showpic，
//3、Java层收到数据后，在主线程中显示。

extern "C"
JNIEXPORT void
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testByteBuffer(JNIEnv* env,
                                                             jobject me)
{
    //TODO...
    return ;
}


//测试INT数组
//1、在JNI层new数组，传输到java层，java层打印出来。
extern "C"
JNIEXPORT jintArray
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testNewArray(JNIEnv* env,
                                                           jobject me)
{
    jintArray array = env->NewIntArray(10);
    jint* content = new jint[10];
    for(int i=0; i<10; i++)
    {
        content[i]=i*2;
    }
//    env->SetIntArrayRegion(array,0,10,content);//
    env->ReleaseIntArrayElements(array,content,0);

//    delete[] content;
    content = NULL;
    return array;
}

jint*  m_jintarrry;
jint arraylen;
//2、把数据从java层传输下去，随后在jni层修改，再返回给java层显示
extern "C"
JNIEXPORT void
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testSetArrayInt(JNIEnv* env,
                                                              jobject me,jintArray  testintarray)
{
    m_jintarrry = env->GetIntArrayElements(testintarray,NULL);//jni没有提供jintarr类型数据的拷贝操作。
    arraylen = env->GetArrayLength(testintarray);
    if(m_jintarrry == NULL)
    {
        TLOGE("跪下了....");
    }

}

extern "C"
JNIEXPORT jintArray
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testGetArrayInt(JNIEnv* env,
                                                              jobject me)
{

    jintArray  j = env->NewIntArray(arraylen);//new一个新的array；new完毕后，。
    env->ReleaseIntArrayElements(j,m_jintarrry,0);//释放并且内容拷贝回到java层；
    return j;//这个对象交给java管理-GC是否
}

//test java->jni object
extern "C"
JNIEXPORT jfloat
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testObjectAdd(JNIEnv* env,
                                                            jobject me,jobject test)
{
    //获取类
    jclass  clazz = env->GetObjectClass(test);
    //methodid
    jmethodID  mid = env->GetMethodID(clazz,"sum","(II)F");
    //call method
    jfloat  jf = env->CallFloatMethod(test,mid,1,2);

    return jf;
}
extern "C"
JNIEXPORT jfloat
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testObjectDiv(JNIEnv* env,
                                                            jobject me,jobject test)
{
    //获取类
    jclass  clazz = env->GetObjectClass(test); //1、获取类
    //methodid
    jmethodID  mid = env->GetMethodID(clazz,"div","(II)F");//2、获取方法id
    //call method
    jfloat  jf = env->CallFloatMethod(test,mid,1,0);//3、调用

    return jf;
}
extern "C"
JNIEXPORT jfloat
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testObjectSub(JNIEnv* env,
                                                            jobject me,jobject test)
{
    //获取类
    jclass  clazz = env->GetObjectClass(test);
    //methodid
    jmethodID  mid = env->GetMethodID(clazz,"sub","(II)F");
    //call method
    jfloat  jf = env->CallFloatMethod(test,mid,1,2);

    return jf;
}


/**
 * 测试JNI中局部引用数量
 */
extern "C"
JNIEXPORT void
JNICALL
        Java_qz_gl_gx_test_1native_1java_MainActivity_testMaxLocalDef(JNIEnv* env,
                                                                      jobject me)
{

    jclass c1 = (*env).FindClass("java/lang/String");
    jclass c2 = (*env).FindClass("java/lang/String");
    jclass c3 = (*env).FindClass("java/lang/String");
    jclass c4 = (*env).FindClass("java/lang/String");
    jclass c5 = (*env).FindClass("java/lang/String");
    jclass c6 = (*env).FindClass("java/lang/String");
    jclass c7 = (*env).FindClass("java/lang/String");
    jclass c8 = (*env).FindClass("java/lang/String");
    jclass c9 = (*env).FindClass("java/lang/String");
    jclass c10 = (*env).FindClass("java/lang/String");
    jclass c11 = (*env).FindClass("java/lang/String");
    jclass c12 = (*env).FindClass("java/lang/String");
    jclass c13 = (*env).FindClass("java/lang/String");
    jclass c14 = (*env).FindClass("java/lang/String");
    jclass c15 = (*env).FindClass("java/lang/String");
    jclass c16 = (*env).FindClass("java/lang/String");
    jclass c17 = (*env).FindClass("java/lang/String");
    jclass c1111 = (*env).FindClass("java/lang/String");
    jclass c18 = (*env).FindClass("java/lang/String");
    jclass c19 = (*env).FindClass("java/lang/String");
    jclass c20 = (*env).FindClass("java/lang/String");
    jclass c21 = (*env).FindClass("java/lang/String");
    jclass c22 = (*env).FindClass("java/lang/String");
    jclass c23 = (*env).FindClass("java/lang/String");
    jclass c24 = (*env).FindClass("java/lang/String");
    jclass c25 = (*env).FindClass("java/lang/String");
    jclass c26 = (*env).FindClass("java/lang/String");
    jclass c27 = (*env).FindClass("java/lang/String");
    jclass c28 = (*env).FindClass("java/lang/String");
    jclass c29 = (*env).FindClass("java/lang/String");
    jclass c30 = (*env).FindClass("java/lang/String");
    jclass c31 = (*env).FindClass("java/lang/String");
    jclass c32 = (*env).FindClass("java/lang/String");
    jclass c33 = (*env).FindClass("java/lang/String");
    jclass c34 = (*env).FindClass("java/lang/String");
    jclass c35 = (*env).FindClass("java/lang/String");
    jclass c36 = (*env).FindClass("java/lang/String");
    jclass c37 = (*env).FindClass("java/lang/String");

    env->DeleteLocalRef(c1);
    env->DeleteLocalRef(c2);
    env->DeleteLocalRef(c3);
    env->DeleteLocalRef(c4);
    env->DeleteLocalRef(c5);
    env->DeleteLocalRef(c6);
    env->DeleteLocalRef(c7);
    env->DeleteLocalRef(c8);
    env->DeleteLocalRef(c9);
    env->DeleteLocalRef(c10);
    env->DeleteLocalRef(c11);
    env->DeleteLocalRef(c12);
    env->DeleteLocalRef(c13);
    env->DeleteLocalRef(c14);
    env->DeleteLocalRef(c15);
    env->DeleteLocalRef(c16);
    env->DeleteLocalRef(c17);
    env->DeleteLocalRef(c1111);
    env->DeleteLocalRef(c19);
    env->DeleteLocalRef(c20);
    env->DeleteLocalRef(c21);
    env->DeleteLocalRef(c22);
    env->DeleteLocalRef(c23);
    env->DeleteLocalRef(c24);
    env->DeleteLocalRef(c25);
    env->DeleteLocalRef(c26);
    env->DeleteLocalRef(c27);
    env->DeleteLocalRef(c28);
    env->DeleteLocalRef(c29);
    env->DeleteLocalRef(c30);
    env->DeleteLocalRef(c31);
    env->DeleteLocalRef(c32);
    env->DeleteLocalRef(c33);
    env->DeleteLocalRef(c34);
    env->DeleteLocalRef(c35);

    for(int i=0; i<10000; i++)//测试flyme系统android7.1.1 局部变量 504个 再多一个局部变量，程序就崩溃
        //但是程序提示 超过了512个局部变量，所有需要计算传入参数其他的局部变量。
    {
        TLOGE(" %d",i);
        jclass c38_New_ref = (jclass )env->NewLocalRef(c37);

        //上述数据是没有是否的时候，是否就不出现变量溢出问题了。
        env->DeleteLocalRef(c38_New_ref);
    }


    for(int i=0; i<10000; i++)//测试flyme系统android7.1.1 局部变量 504个 再多一个局部变量，程序就崩溃
        //但是程序提示 超过了512个局部变量，所有需要计算传入参数其他的局部变量。
    {
        TLOGE(" %d",i);
        jclass c38_New_ref = (jclass )env->NewLocalRef(c37);

        //上述数据是没有是否的时候，是否就不出现变量溢出问题了。 这是推荐的使用方式。
        env->DeleteLocalRef(c38_New_ref);
    }

    env->DeleteLocalRef(c37);

    TLOGE("Java_qz_gl_gx_test_1native_1java_MainActivity_testMaxLocalDef JNI layer print max local ref , over 37 local ref variables");


    return;
}



jobject  jsynchronizedobj;
int iRes = 0;
void *run_read_thread(void*);
void *run_write_thread(void*);
pthread_t pthread_1,pthread_2;
pthread_attr_t pa_1,pa_2;
/**
 * test multi-thread co-work
 * one read,one write(output) by log/main-ui thtread
 */
extern "C"
JNIEXPORT void
JNICALL
Java_qz_gl_gx_test_1native_1java_MainActivity_testMultiThreadCowork(JNIEnv* env,
                                                              jobject me)
{

    jclass exclazz = env->FindClass("java/Lang/Excepiton");
    if(exclazz == 0)
    {
        TLOGD("Crash when FindClass!");
        return ;
    }
    //use varibale(env) to synchronized public resource
    jsynchronizedobj = env->NewGlobalRef(exclazz);

    //first of all get JVM
    JavaVM* jvm;
    jint iRet = env->GetJavaVM(&jvm);

    if(iRet < 0)
    {
        //ERROR
        env->ThrowNew(exclazz, "GetJavaVM return < 0!");
        return;
    }


    pthread_attr_init(&pa_1);
    pthread_attr_init(&pa_2);
    pthread_create(&pthread_1,&pa_1,run_write_thread,NULL);
    pthread_create(&pthread_2,&pa_1,run_read_thread,NULL);

}


void *run_read_thread(void*)
{//output log
    for(int i = 0; i<100000; i++)
    {

    }
}

void *run_write_thread(void*)
{

}
