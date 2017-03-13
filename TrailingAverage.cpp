#include "TrailingAverage.h"

TrailingAverage::TrailingAverage(int trailingWindowSize){
    _numItems = trailingWindowSize;
    _index = 0;
    _total = 0;
	_values = new int[_numItems];
   for(int i=0; i<_numItems; i++) { _values[i] = 0; }
}

/*=========================================================================
 * averageInput - computes the average of trailing X input values to
 *    hide the noise inherent in Analog.
 * Algortihm:
 *    To avoid having to iterate through the array and recomputing the total
 *    every time, we keep the total from call to call and then subtract the 
 *    value being replaced and add the replacement value being added.
 * Setup: 
 *    _numItems: the number of trailing window inputs to track
 *    _values: an array[_numItems] that holds the values
 *    _total: the current total value
 *    _index: the current location of the item to replace
 * Parameters:
 *    input: the new input to add to the average
 * Returns:
 *    the computed average value
 *=========================================================================
 */
int TrailingAverage::averageInput(int input){
  
  int newAvg = 0;

  _total = _total - _values[_index];
  if(_total < 0) { _total = 0; }
  
  _values[_index] = input;
  _total += input;
  
  newAvg = _total/_numItems;
  
  _index++;
  if(_index > _numItems-1) { _index = 0; }
  
  return(newAvg);
} // averageInput

