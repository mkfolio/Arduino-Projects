package smokinbear.appbeta1;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.MediaController;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.widget.VideoView;

import java.io.IOException;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    String address = null;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    ImageButton right;
    ImageButton left;
    ImageButton up;
    ImageButton down;
    ImageButton stop;
    ToggleButton selection;
    private String TAG = "Hello:";
    EditText addrField;
    Button btnConnect;
    Button dc;
    VideoView streamView;
    MediaController mediaController;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Intent addint = getIntent();
        address = addint.getStringExtra(DeviceList.EXTRA_ADDRESS);

        addrField = (EditText)findViewById(R.id.addr);
        btnConnect = (Button)findViewById(R.id.connect);
        streamView = (VideoView)findViewById(R.id.streamview);
        right = (ImageButton) findViewById(R.id.RIGHT);
        left = (ImageButton) findViewById(R.id.LEFT);
        up = (ImageButton) findViewById(R.id.UP);
//        down = (ImageButton) findViewById(R.id.DOWN);
        stop = (ImageButton) findViewById(R.id.CENTER);
        dc = (Button) findViewById(R.id.dc);
        selection = (ToggleButton) findViewById(R.id.selection) ;
        new ConnectBT().execute();

        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String s = addrField.getEditableText().toString();
                playStream(s);
            }
        });

        selection.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    autoMode();
                }
                else {
                    manualMode();
                }
            }
        });

        right.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                right();
                Log.d(TAG,"RIGHT");
            }
        });

        up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                up();
                Log.d(TAG,"UP");
            }
        });

//        down.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                down();
//                Log.d(TAG,"DOWN");
//            }
//        });

        left.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                left();
                Log.d(TAG,"LEFT");
            }
        });

        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stop();
                Log.d(TAG,"stop");
            }
        });

        dc.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Disconnect(); //close connection
            }
        });

    }

    private void Disconnect()
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.close(); //close connection
            }
            catch (IOException e)
            { msg("Error");}
        }
        finish(); //return to the first layout

    }

    private void playStream(String src){
        Uri UriSrc = Uri.parse(src);
        if(UriSrc == null){

        } else {
            streamView.setVideoURI(UriSrc);
            mediaController = new MediaController(this);
            streamView.setMediaController(mediaController);
            streamView.start();

            Toast.makeText(MainActivity.this,
                    "Connect: " + src,
                    Toast.LENGTH_LONG).show();
        }
    }

    private void autoMode()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("Q".getBytes());
            }
            catch (IOException e)
            { msg("Error");}
        }

    }

    private void manualMode()
    {
        if (btSocket!=null)
        {
            try
            {

                btSocket.getOutputStream().write("E".getBytes());
            }
            catch (IOException e)
            { msg("Error");}
        }

    }



    private void up()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("W".getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void left()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("A".getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void right()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("D".getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void down()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("S".getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void stop()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("X".getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }



    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(MainActivity.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Bluetooth not connected.");
                finish();
            }
            else
            {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        streamView.stopPlayback();
    }


}
