package sorting;

import java.util.ArrayList;
import java.util.Comparator;

/**
 * Implements the insertion sort altorithm.
 */
class InsertionSort {
  /**
   * inserts element array[pos] in order into array [0..pos-1] shifting all
   * following elements to make place for the inserted element.
   *
   * It assumes that array[0..pos-1] is sorted.
   * @method insert
   * @param  array: the array holding the elements
   * @param  pos: the position of the element to be inserted
   */
  public static <T> void insert(ArrayList<T> array, int pos, Comparator<T> comparator) {
    T elem = array.get(pos);
    int i;
    for(i=pos-1; i>=0 && comparator.compare(array.get(i), elem) > 0; --i ) {
      array.set(i+1, array.get(i));
    }

    array.set(i+1, elem);
  }

  /**
   * sorts the given array
   * @method sort
   * @param  array The array to be sorted
   */
  public static <T> void sort(ArrayList<T> array, Comparator<T> comparator) {
    for(int i=1; i<array.size(); ++i) {
      insert(array, i, comparator);
    }
  }
}
