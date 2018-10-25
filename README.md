# fast-EADAM
A solver for college admission problem with consent based on paper 'Legal Assignments and fast EADAM with consent via classical theory of stable matchings'.

Warning: It is still in construction. The correctness and efficiency are not guaranteed.

## How to use
Please check the test folder for examples of input and output.
The input file should contain the information below:

1. num_of_students (n), num_of_schools (m);
2. m numbers of seats of schools;
3. n numbers of students consenting or not (1: yes; 0: no);
4. a n * m matrix representing preference of students;
5. a m * n matrix representing preference of schools.

## Current Progress
Build the basic data structure;

Implement Gale-Shapley Algorithm in O(E) time and space complexity;

Implememt fast EADAM algorithm in a all-consenting special case in O(E) time and space complexity. 

## TODO
Add more comments;

Implement the full fast EADAM.

## License
[MIT](https://opensource.org/licenses/MIT)

## Reference
For the origin paper that published the algorithm, please refer to https://arxiv.org/pdf/1809.08506.

