package com.xiaofei.test;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HTTP;
import org.apache.http.util.EntityUtils;

import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
	private Button btn;
	private TextView tv;
	private TextView response;
	private String strResponse;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		btn = (Button) findViewById(R.id.start_btn);
		tv = (TextView) findViewById(R.id.content);
		response = (TextView) findViewById(R.id.response);
		btn.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				btn.setEnabled(false);
				update();
			}
		});
		strResponse = "";
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	protected void httpTest() {
		// URLַ
		// String uriAPI =
		// "http://www.dubblogs.cc:8751/Android/Test/API/Post/index.php";
		String uriAPI = "http://192.168.1.37/web/getservices";
		/* 建立HTTP Post连线 */
		HttpPost httpRequest = new HttpPost(uriAPI);
		// Post运作传送变数必须用NameValuePair[]阵列储存
		// 传参数 服务端获取的方法为request.getParameter("name")
		List<NameValuePair> params = new ArrayList<NameValuePair>();
		params.add(new BasicNameValuePair(
				"sRef",
				"1:7:1:0:0:0:0:0:0:0:%28type%20==%201%29%20||%20%28type%20==%2017%29%20||%20%28type%20==%20195%29%20||%20%28type%20==%2025%29%20ORDER%20BY%20name"));
		try {

			// 发出HTTP request
			httpRequest.setEntity(new UrlEncodedFormEntity(params, HTTP.UTF_8));
			// 取得HTTP response
			HttpResponse httpResponse = new DefaultHttpClient()
					.execute(httpRequest);

			// 若状态码为200 ok
			if (httpResponse.getStatusLine().getStatusCode() == 200) {
				// 取出回应字串
				String strResult = EntityUtils.toString(httpResponse
						.getEntity());
				strResponse = strResult;
			} else {
				strResponse = "Error Response"
						+ httpResponse.getStatusLine().toString();
			}
		} catch (ClientProtocolException e) {
			strResponse = e.getMessage().toString();
			e.printStackTrace();
		} catch (UnsupportedEncodingException e) {
			strResponse = e.getMessage().toString();
			e.printStackTrace();
		} catch (IOException e) {
			strResponse = e.getMessage().toString();
			e.printStackTrace();
		}
	}

	private void update() {
		strResponse = "";
		UpdateTextTask updateTextTask = new UpdateTextTask(this);
		updateTextTask.execute();
	}

	class UpdateTextTask extends AsyncTask<Void, Integer, Integer> {
		private Context context;

		UpdateTextTask(Context context) {
			this.context = context;
		}

		/**
		 * 运行在UI线程中，在调用doInBackground()之前执行
		 */
		@Override
		protected void onPreExecute() {
			Toast.makeText(context, "开始执行", Toast.LENGTH_SHORT).show();
			response.setText(strResponse);
		}

		/**
		 * 后台运行的方法，可以运行非UI线程，可以执行耗时的方法
		 */
		@Override
		protected Integer doInBackground(Void... params) {
			int i = 0;
			while (i < 3) {
				i++;
				publishProgress(i);
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
				}
			}
			httpTest();
			return null;
		}

		/**
		 * 运行在ui线程中，在doInBackground()执行完毕后执行
		 */
		@Override
		protected void onPostExecute(Integer integer) {
			Toast.makeText(context, "执行完毕", Toast.LENGTH_SHORT).show();
			response.setText(strResponse);
			btn.setEnabled(true);
		}

		/**
		 * 在publishProgress()被调用以后执行，publishProgress()用于更新进度
		 */
		@Override
		protected void onProgressUpdate(Integer... values) {
			tv.setText("" + values[0]);
		}
	}
}