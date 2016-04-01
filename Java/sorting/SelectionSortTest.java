package sorting;

import static org.junit.Assert.*;
import org.junit.Test;
import org.junit.Before;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Arrays;



public class SelectionSortTest {
  ArrayList<Integer> array;

  class IntComparator implements Comparator<Integer> {
    public int compare(Integer i1, Integer i2) {
      return i1.compareTo(i2);
    }
  }

  @Before
  public void setup() {
    array = new ArrayList<Integer>();
    array.add(10);
    array.add(5);
    array.add(8);
    array.add(11);
    array.add(1);
    array.add(4);
  }

  @Test
  public void testSortEmptyArray() {
    ArrayList<String> array = new ArrayList<String>();
    SelectionSort.sort(array, new Comparator<String>() {
      public int compare(String s1, String s2) {
        return s1.compareTo(s2);
      }
    });
  }

  @Test
  public void testSortFullArray() {
    Integer[] expected = new Integer[]{1,4,5,8,10,11};
    SelectionSort.sort(array, new IntComparator());
    assertEquals( new ArrayList<Integer>(Arrays.asList(expected)), array );
  }

  @Test
  public void testSwap() {
    SelectionSort.swap(array, 1, 5);
    assertEquals(4, (int) array.get(1));
    assertEquals(5, (int) array.get(5));
  }

  @Test
  public void testSelect() {
    int min_index = SelectionSort.select(array, 1, new IntComparator());

    assertEquals(4, min_index);
  }

}
