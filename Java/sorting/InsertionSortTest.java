package sorting;

import org.junit.Test;
import org.junit.Before;
import static org.junit.Assert.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;


public class InsertionSortTest {
  ArrayList<Integer> array;

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
    InsertionSort.sort(array, new Comparator<String>() {
      public int compare(String s1, String s2) {
        return s1.compareTo(s2);
      }
    });
  }

  @Test
  public void testSortFullArray() {
    Integer[] expected = new Integer[]{1,4,5,8,10,11};
    InsertionSort.sort(array, new Comparator<Integer>() {
      public int compare(Integer i1, Integer i2) {
        return i1.compareTo(i2);
      }
    });
    assertEquals( new ArrayList<Integer>(Arrays.asList(expected)), array );
  }
}
