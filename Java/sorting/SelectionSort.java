package sorting;

import java.util.ArrayList;
import java.util.Comparator;

class SelectionSort {
  static <T> int select(ArrayList<T> array, int start, Comparator<T> comparator) {
    int min_pos = start;
    for(int i=start+1; i<array.size(); ++i) {
      if(comparator.compare(array.get(i), array.get(min_pos)) < 0) {
        min_pos = i;
      }
    }

    return min_pos;
  }

  static <T> void swap(ArrayList<T> array, int i, int j) {
    T tmp = array.get(i);
    array.set(i, array.get(j));
    array.set(j, tmp);
  }

  public static <T> void sort(ArrayList<T> array, Comparator<T> comparator) {
    for(int i=0; i<array.size()-1; ++i) {
      int min_pos = select(array, i, comparator);
      swap(array, i, min_pos);
    }
  }

}
