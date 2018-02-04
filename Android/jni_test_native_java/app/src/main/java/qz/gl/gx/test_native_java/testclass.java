package qz.gl.gx.test_native_java;

/**
 * Created by tangss on 2018/1/30.
 */

public class testclass
{

    private MainActivity m_mainActivity;

    public testclass(MainActivity mainActivity) {
        m_mainActivity = mainActivity;
    }

    public float sum(int a, int b)
    {
//        m_mainActivity.showContent("sum:"+(a+b));
        return a+b;
    }

    private float sub(int a,int div)//test call private method'behaviour in jni ; can be called in jni by run test
    {
//        m_mainActivity.showContent("sub:"+(a-div));

        return a-div;
    }

    public float div(int a, int b)//test exception in jni when b=0
    {
//        m_mainActivity.showContent("subdiv:"+(a/b));
        return a/b;
    }

}
