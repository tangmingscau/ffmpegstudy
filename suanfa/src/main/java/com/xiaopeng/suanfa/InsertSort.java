package com.xiaopeng.suanfa;

import java.util.Arrays;

/**
 * 插入排序
 *
 * @author tony
 * @date 2019/3/4
 */
public class InsertSort {
    public static void main(String[] args) {
        int[] array = DataProvider.generateRandomArray(10000, 0, 10000);
        int[] array2 = {1, 2, 3, 4, 5, 6, 7, 8, 10, 9};
        int[] array3 = {1, 2, 5, 7, 4, 3, 6, 8, 9, 10};
        System.out.println("插入排序\n");
        long start = System.currentTimeMillis();
        insertSort(array.clone());
        System.out.println("插入排序耗时" + (System.currentTimeMillis() - start) + "\n");
        System.out.println("插入排序2\n");
        start = System.currentTimeMillis();
        insertSort2(array.clone());
        System.out.println("插入排序2耗时" + (System.currentTimeMillis() - start) + "\n");
    }

    /**
     * 插入排序，很像冒泡，但是有所区别，冒泡是每次都将最大或者最小数放到排序头
     * 插入时，每次所要处理的数据，不一定是放在排序头，或者是插到中，前，后都有可能
     *
     * @param toSorts
     */
    public static void insertSort(int[] toSorts) {
        int count = 0;
        for (int i = 1; i < toSorts.length; i++) {
            //从1开始，因为只有一个数据时，他本身就是有序的
            for (int j = i - 1; j >= 0; j--) {
                //选择合适的插入位置,a[i]就是待插入数据
                if (toSorts[j + 1] < toSorts[j]) {
                    int temp = toSorts[j + 1];
                    toSorts[j + 1] = toSorts[j];
                    toSorts[j] = temp;
                }
                count++;
            }

        }
        System.out.println("count=" + count + " " + Arrays.toString(toSorts) + "\n");
    }

    /**
     * 插入排序，在之前的基础上优化
     * 由于a[0,i-1]是有序的，若是a[i]与a[i-1]不交换数据，说明a[0,i]也是有序的，那么就无需对比剩下的数据了
     *
     * @param toSorts
     */
    public static void insertSort2(int[] toSorts) {
        int count = 0;
        for (int i = 1; i < toSorts.length; i++) {
            //从1开始，因为只有一个数据时，他本身就是有序的
            for (int j = i - 1; j >= 0; j--) {
                //选择合适的插入位置,a[i]就是待插入数据
                if (toSorts[j + 1] < toSorts[j]) {
                    int temp = toSorts[j + 1];
                    toSorts[j + 1] = toSorts[j];
                    toSorts[j] = temp;
                } else {
                    //由于a[0,i-1]是有序的，若是a[i]与a[i-1]不交换数据，说明a[0,i]也是有序的，那么就无需对比剩下的数据了
                    break;
                }
                count++;
            }

        }
        System.out.println("count=" + count + " " + Arrays.toString(toSorts) + "\n");
    }
}
