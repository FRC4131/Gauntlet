
/*==================================================================================================
 * TrailingAverage: reading analog inputs is often troublesome due to the inherent noise that
 *   is the nature of Analog. This is a simple trailing window averaging algorithm that keeps
 *   the last N datapoints and averages across those.
 *================================================================================================== 
 */
class TrailingAverage{
  public:
    TrailingAverage(int trailingWindowSize);
	int averageInput(int input);

  private:
    int _numItems;
    int _index;
    int _total;
	int *_values;  // pointer to a dynamically allocated array of constructor specified size
};
