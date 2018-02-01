# -*- coding: utf-8 -*-
#!/usr/bin/env python
#================================================================
#   Copyright (C) 2018年02月01日 肖飞 All rights reserved
#   
#   文件名称：test_in_top_k.py
#   创 建 者：肖飞
#   创建日期：2018年02月01日 星期四 10时03分41秒
#   修改日期：2018年02月01日 星期四 11时08分01秒
#   描    述：
#
#================================================================
import tensorflow as tf

def main():
    logits = tf.Variable(tf.truncated_normal(shape=[10,5],stddev=1.0))#logits表示一批中的各条各类别分布权重
    labels = tf.constant([0,4,0,0,0,0,0,0,0,0])#代表每条实例实际类别索引[0-4]

    top_1_op = tf.nn.in_top_k(logits,labels,1)#预测类别（最高权重）中包含实际类别，为True
    top_2_op = tf.nn.in_top_k(logits,labels,2)#预测类别（最高,次高权重）中包含实际类别，为True

    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        print(logits.eval())
        print(labels.eval())
        print(top_1_op.eval())
        print(top_2_op.eval())

if '__main__' == __name__:
    main()
