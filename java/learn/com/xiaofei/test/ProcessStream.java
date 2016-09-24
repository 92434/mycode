package com.xiaofei.test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

class waitThread extends Thread {
    Process p = null;
    boolean wait = true;

    waitThread(Process process) {
        p = process;

        wait = true;
    }

    public void run() {
        try {
            p.waitFor();
            //Thread.sleep(1000);
        } catch (Exception ex) {
            ex.printStackTrace();
        }


        wait = false;
    }

    public boolean needWait() {
        return wait;
    }
}

public class ProcessStream {


    public static void main(String[] args) {
        try {
            // create a new process
            Process p = Runtime.getRuntime().exec(args[0]);

            waitThread wait = new waitThread(p);
            //wait.start();

            // get the input stream of the process and print it
            InputStream in = p.getInputStream();
            //InputStream in = p.getErrorStream();
            InputStreamReader isr = new InputStreamReader(in);
            BufferedReader br = new BufferedReader(isr);

            //String line = null;
            String line = "ok";

            //while(wait.needWait()) {
            while(line != null) {
                line = br.readLine();

                if(null != line) {
                    System.out.println(line);
                }
            }


            p.destroy();

            br.close();
            isr.close();
    } catch (Exception ex) {
        ex.printStackTrace();
    }
}

}
