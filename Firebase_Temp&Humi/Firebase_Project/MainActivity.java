package com.example.firabase_project;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.Button;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    private WebView webView;
    Button reBtn;

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        reBtn = (Button)findViewById(R.id.reBtn);
        reBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                webView.reload();
            }
        });

        webView = (WebView)findViewById(R.id.webVIew);
        String webUrlLocal = "file:///android_asset/Firebase_Project_201954659.html";

        webView.setWebChromeClient(new WebChromeClient());

        WebSettings webSettings = webView.getSettings();
//        webSettings.setWebChromeClient(chromeClient);
        webSettings.setCacheMode(WebSettings.LOAD_NO_CACHE);
        webSettings.setBuiltInZoomControls(true);
        webSettings.setSupportZoom(true);
        webSettings.setJavaScriptEnabled(true);
//        webSettings.setDomStorageEnabled(true);
        webSettings.setAllowFileAccess(true);
//        webSettings.setMixedContentMode(WebSettings.MIXED_CONTENT_ALWAYS_ALLOW);

        webView.loadUrl(webUrlLocal);

    }
}
