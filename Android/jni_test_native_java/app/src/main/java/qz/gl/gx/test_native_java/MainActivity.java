package qz.gl.gx.test_native_java;

import android.support.annotation.UiThread;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.nio.ByteBuffer;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;


public class MainActivity extends AppCompatActivity {


    @BindView(R.id.intText)
    EditText m_intText;

    @BindView(R.id.charText)
    EditText m_charText;


    @BindView(R.id.outputresult)
    TextView m_outputresult;
    @BindView(R.id.test)
    Button m_btn_test;

    @BindView(R.id.test_string)
    Button m_btn_test_string;

    @BindView(R.id.stringText)
    EditText m_edit_stringText;

    @BindView(R.id.btn_jni_set_str)
    Button m_btn_jnisetstr;

    @BindView(R.id.test_jni_int_array)
    Button m_btn_test_jni_int_array;

    @BindView(R.id.btn_test_object)
    Button m_btn_test_object;

    @BindView(R.id.test_btn_max_localref)
    Button m_btn_test_btn_max_localref;

    private String m_txt_prop;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private boolean TEST_LEAK = false;
    private testclass mTestClassObj = new testclass(this);
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ButterKnife.bind(MainActivity.this);

        m_btn_test.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sayHi(m_btn_test);
            }
        });


    }

    @OnClick(R.id.test)
    public void sayHi(Button btn)
    {
        Log.e("TSS","int input:"+m_intText.getText().toString());
        Log.e("TSS","char input:"+m_charText.getText().toString());

        if(m_intText.getText().length() <= 0)
        {
            Toast.makeText(MainActivity.this,"整数输入为空，请输入", Toast.LENGTH_SHORT).show();
            return;
        }
        if(m_charText.getText().length() <= 0)
        {
            Toast.makeText(MainActivity.this,"字符输入为空，请输入", Toast.LENGTH_SHORT).show();
            return;
        }

        char c = m_charText.getText().charAt(0);
        setCharPara(c);

        setIntPara(Integer.parseInt(m_intText.getText().toString()));

        String content = "读取JNI int内容 :"+getIntPara() + "\n\r读取JNI Char内容 :"+getCharPara();
        m_outputresult.setText(content);
        m_outputresult.setVisibility(View.VISIBLE);
    }

    @OnClick(R.id.test_string)
    public void Test_String(Button btn)
    {

        setStringIntoJni(m_edit_stringText.getText().toString());
        Log.e("TSS","Test_String input:"+m_edit_stringText.getText().toString());
        if(m_edit_stringText.getText().length() <= 0)
            Toast.makeText(MainActivity.this,"输入为空，请输入", Toast.LENGTH_SHORT).show();
        String content = "从JNI获取内容 :"+returnStringToJava();
        m_outputresult.setText(content);
        Log.e("TSS","从JNI获取的内容: "+content);
        m_outputresult.setVisibility(View.VISIBLE);
    }

    @OnClick(R.id.btn_jni_set_str)
    public void a1(Button btn)
    {
        String txt = getJNINewString();
        String content = "从JNI获取内容 :"+txt;
        m_outputresult.setText(content);
        Log.e("TSS","从JNI获取的字符串内容: "+content);
        m_outputresult.setVisibility(View.VISIBLE);
    }

    @OnClick(R.id.test_jni_int_array)
    public void btn_test_jni_int_array_click(Button btn)
    {
        if(!TEST_LEAK)
        {

            int[] intarray= testNewArray();
            String intarrayStr = printIntArray(intarray);

            Log.e("TSS","ARRAY:"+ printIntArray(intarray));
            String content = "从JNI获取内容 :"+intarrayStr;


            intarray[5] = 0;
            content += "   \n\r设置新数组到JNI中:"+printIntArray(intarray);
            testSetArrayInt(intarray);
            intarray = testGetArrayInt();
            content += "   \n\r再从JNI中获取数组:"+printIntArray(intarray);
            m_outputresult.setText(content);

        }else
        {//内存测试；配置ps|grep qz.gl.gx.test_native_java 查看内存结果
            new Thread(new Runnable() {
                @Override
                public void run() {
                    for(int i=0; i< 10000; i++)
                    {
                        int[] intarray= testNewArray();
                        String intarrayStr = printIntArray(intarray);

                        Log.e("TSS","ARRAY:"+ printIntArray(intarray));
                        String content = "从JNI获取内容 :"+intarrayStr;


                        intarray[5] = 0;
                        content += "   \n\r设置新数组到JNI中:"+printIntArray(intarray);
                        testSetArrayInt(intarray);
                        intarray = testGetArrayInt();
                        content += "   \n\r再从JNI中获取数组:"+printIntArray(intarray);
                        try {
                            Thread.sleep(50);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                }
            }).start();

        }


    }

    @OnClick(R.id.btn_test_object)
    public void _1(Button btn)
    {
       float f = testObjectAdd(this.mTestClassObj);
        String content = "从JNI获取内容 :"+" add:"+f;
        m_outputresult.setText(content);
        m_outputresult.setVisibility(View.VISIBLE);

        f = testObjectSub(this.mTestClassObj);  //私有方法能被JNI访问...屌屌的哦，在JNI不同类中可以访问private方法
        content += " Sub :"+f;
        m_outputresult.setText(content);
        m_outputresult.setVisibility(View.VISIBLE);

        try
        {
            f = testObjectDiv(this.mTestClassObj); //jni的异常可以被在java访问
            content += " Div :"+f;
        }catch (Exception ex)
        {
            Log.e("TSS", ex.getLocalizedMessage());
        }

        m_outputresult.setText(content);
        m_outputresult.setVisibility(View.VISIBLE);
    }

    @OnClick(R.id.test_btn_max_localref)
    public void a2(Button btn)
    {
        Log.e("TSS_LOG","test test_btn_max_localref java layer begin");
        testMaxLocalDef();
        Log.e("TSS_LOG","test test_btn_max_localref  java layer end");
    }

    public void showpic(ByteBuffer bb)
    {

    }

    private  String printIntArray(int[] ia)
    {
        String ret = "";
        for (int i:ia) {

            ret += " "+i;

        }
        return ret;
    }




    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String returnStringToJava();
    public native void setStringIntoJni(String strParam);
    public native String getJNINewString();

    public native void setIntPara(int intpara);
    public native void setCharPara(char charparam);
    public native int  getIntPara();
    public native char getCharPara();

    //测试byte jni从网络中获取图片，然后传到java层显示；
    //涉及：
    //1、jni开线程从网络获取资源
    //2、把数据传输到java层 - public void showpic，
    //3、Java层收到数据后，在主线程中显示。
    public native void testByteBuffer();


    //测试INT数组
    //1、在JNI层new数组，传输到java层，java层打印出来。
    public native int[] testNewArray();
    //2、把数据从java层传输下去，随后在jni层修改，再返回给java层显示
    public native void testSetArrayInt(int[] testintarray);

    public native int[] testGetArrayInt();


    //test java->jni object
    public native float testObjectAdd(testclass test);
    public native float testObjectDiv(testclass test);
    public native float testObjectSub(testclass test);

    //test max local ref
    public  native float testMaxLocalDef();

}
