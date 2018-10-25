package com.xiaopeng.ffmpegstudy.thread;

public class Consumer extends Thread {

    private Producer producer;

    public Consumer(String name, Producer producer) {
        super(name);
        this.producer = producer;
    }

    @Override
    public void run() {
        Message msg = producer.waitMsg();
        System.out.println("Consumer " + getName() + " get a msg");
    }

}

