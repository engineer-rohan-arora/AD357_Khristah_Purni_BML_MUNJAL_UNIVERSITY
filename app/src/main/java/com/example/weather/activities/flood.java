package com.example.weather.activities;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.TextView;

import com.example.weather.activities.BaseActivity;

import org.tensorflow.lite.Interpreter;

import java.io.FileInputStream;
import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;

import com.example.weather.R;

public class flood extends BaseActivity {

    NumberPicker nPicker1,nPicker2;
    TextView result_tv;
    Button xor_bt;
    String modelFile="xorGate.lite";
    Interpreter tflite;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_flood);
        nPicker1 =findViewById(R.id.np1);
        nPicker2 =findViewById(R.id.np2);
        result_tv=findViewById(R.id.result_tv);
        xor_bt=findViewById(R.id.bt);

        nPicker1.setMinValue(0);
        nPicker1.setMaxValue(1);
        nPicker2.setMinValue(0);
        nPicker2.setMaxValue(1);
        try {
            tflite = new Interpreter(loadModelFile(this, modelFile));
        }
        catch (IOException e) {
            e.printStackTrace();
        }


        xor_bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                float n1 = nPicker1.getValue();
                float n2 = nPicker2.getValue();
                float[][] inp=new float[][]{{n1,n2}};
                float[][] out=new float[][]{{0}};
                tflite.run(inp,out);
                result_tv.setText(String.valueOf(Math.round(out[0][0])));
            }

        });
    }

    private MappedByteBuffer loadModelFile(Activity activity,String MODEL_FILE) throws IOException {
        AssetFileDescriptor fileDescriptor = activity.getAssets().openFd(MODEL_FILE);
        FileInputStream inputStream = new FileInputStream(fileDescriptor.getFileDescriptor());
        FileChannel fileChannel = inputStream.getChannel();
        long startOffset = fileDescriptor.getStartOffset();
        long declaredLength = fileDescriptor.getDeclaredLength();
        return fileChannel.map(FileChannel.MapMode.READ_ONLY, startOffset, declaredLength);
    }
}