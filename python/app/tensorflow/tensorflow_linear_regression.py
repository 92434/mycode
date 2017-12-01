# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2017年12月01日 肖飞 All rights reserved
#   
#   文件名称：linear_regression.py
#   创 建 者：肖飞
#   创建日期：2017年12月01日 星期五 11时53分44秒
#   修改日期：2017年12月01日 星期五 15时54分49秒
#   描    述：
#
#================================================================
import tensorflow as tf
def linear_regression_test():
    # Model parameters
    W = tf.Variable([.3], dtype=tf.float32)
    b = tf.Variable([-.3], dtype=tf.float32)
    # Model input and output
    x = tf.placeholder(tf.float32)
    linear_model = W*x + b
    y = tf.placeholder(tf.float32)

    # loss
    loss = tf.reduce_sum(tf.square(linear_model - y)) # sum of the squares
    # optimizer
    optimizer = tf.train.GradientDescentOptimizer(0.01)
    train = optimizer.minimize(loss)

    # training data
    x_train = [1, 2, 3, 4]
    y_train = [0, -1, -2, -3]
    # training loop
    init = tf.global_variables_initializer()
    sess = tf.Session()
    sess.run(init) # reset values to wrong
    for i in range(1000):
        sess.run(train, {x: x_train, y: y_train})

        # evaluate training accuracy
        curr_W, curr_b, curr_loss = sess.run([W, b, loss], {x: x_train, y: y_train})
        print("[%04d]W: %s b: %s loss: %s"%(i, curr_W, curr_b, curr_loss))

def main():
    linear_regression_test()

if '__main__' == __name__:
    main()
