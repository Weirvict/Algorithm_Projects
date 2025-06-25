#!/usr/bin/python3

from which_pyqt import PYQT_VER

if PYQT_VER == 'PYQT5':
    from PyQt5.QtCore import QLineF, QPointF
elif PYQT_VER == 'PYQT4':
    from PyQt4.QtCore import QLineF, QPointF
else:
    raise Exception('Unsupported Version of PyQt: {}'.format(PYQT_VER))

import math
import time

# Used to compute the bandwidth for banded version
MAXINDELS = 3

# Used to implement Needleman-Wunsch scoring
MATCH = -3  # Match score
INDEL = 5   # Gap penalty
SUB = 1     # Mismatch penalty

"""
This function should create a empty table for dynamic programming

@param a is Sequence a
@param b is Sequence b
@param type is the type of alignment
@return table_values is a 2D array of integers
"""


def create_table_values(a, b, type):
    # Create an empty table with appropriate dimensions based on the type of alignment
    if type == 'banded':
        table_values = [[0 for i in range(2 * MAXINDELS + 1)] for j in range(len(a) + 1)]
    elif type == 'unbanded':
        table_values = [[0 for i in range(len(b) + 1)] for j in range(len(a) + 1)]
    else:
        raise ValueError("Invalid type: {}".format(type))
    return table_values


"""
This function maps index to the table in case of banded alignment

@param i is row index
@param j is column index
@return row and column index of the table
"""


def index_location(i, j):
    return i, j - i + MAXINDELS


"""
This function retrieves value from table 

@param i is row index
@param j is column index
@param table_values is the current 2D array of integers
@return value of the table
"""


def get_index_value(i, j, current_value):
    x, y = index_location(i, j)
    if not 0 <= y < 2 * MAXINDELS + 1:
        return math.inf
    return current_value[x][y]


"""
This function sets value in tables

@param i is row index
@param j is column index
@param table_values is the current 2D array of integers
@return new value of the table
"""


def set_index_value(i, j, current_value):  # O(1)
    x, y = index_location(i, j)
    return current_value[x][y]


class GeneSequencing:

    def __init__(self):
        # Initialize lists to store aligned sequences
        self.x = []  # Stores the aligned sequence of A
        self.y = []  # Stores the aligned sequence of B

    # This is the method called by the GUI.  _sequences_ is a list of the ten sequences, _table_ is a
    # handle to the GUI so it can be updated as you find results, _banded_ is a boolean that tells
    # you whether you should compute a banded alignment or full alignment, and _align_length_ tells you
    # how many base pairs to use in computing the alignment

    """
    This function should perform the Needleman-Wunsch algorithm for sequence alignment
    
    @param current_table is the current values of the table
    @param m is the row index
    @param n is the column index
    @param a is the sequence a
    @param b is the sequence b
    @return results is the score of the alignment
    """

    def needleman_Wunsch_Algorithm(self, current_table, m, n, a, b):
        # Base case: If both indices are 0, the alignment score is 0
        if m == 0 and n == 0:
            return 0

        # Calculate scores for insertion, deletion, and match/mismatch
        scores = []
        if n != 0:
            scores.append(current_table[m][n - 1] + INDEL)
        if m != 0 and n != 0:
            scores.append(current_table[m][n - 1] + (SUB if a[m - 1] != b[n - 1] else MATCH))
        if m != 0:
            scores.append(current_table[m - 1][n] + INDEL)

        # Choose the minimum score among insertion, deletion, and match/mismatch
        results = min(scores)

        # Update aligned sequences based on the chosen operation
        if results == scores[0]:
            self.x.append("-")
            self.y.append(b[n - 1])
            return self.needleman_Wunsch_Algorithm(current_table, m, n - 1, a, b)
        elif results == scores[1]:
            self.x.append(a[m - 1])
            self.y.append(b[n - 1])
            return self.needleman_Wunsch_Algorithm(current_table, m - 1, n - 1, a, b)
        else:
            self.x.append(a[m - 1])
            self.y.append("-")
            return self.needleman_Wunsch_Algorithm(current_table, m - 1, n, a, b)

    """ 
    This function performs the unbanded algorithm for sequences a and b.

    @param a: The sequence A
    @param b: The sequence B
    @return: The score of the unbanded alignment and the aligned sequences
    """

    def unbanded(self, a, b):
        # Clear the aligned sequences
        self.x.clear()
        self.y.clear()

        # Get the lengths of sequences A and B
        m = len(a)
        n = len(b)

        # Create an empty table for dynamic programming
        current_table = create_table_values(a, b, "unbanded")

        # Fill the first row and column of the table with gap penalties
        for i in range(1, m + 1):  # O(m)
            current_table[i][0] = INDEL * i
        for j in range(1, n + 1):  # O(n)
            current_table[0][j] = INDEL * j

        # Fill the rest with the table using dynamic programming
        for i in range(1, m + 1):  # O(m)
            for j in range(1, n + 1):  # O(n)
                # Calculate the score for each cell based on the adjacent cells
                diff = SUB if a[i - 1] != b[j - 1] else MATCH
                result = min(current_table[i - 1][j] + INDEL, current_table[i][j - 1] + INDEL,
                             current_table[i - 1][j - 1] + diff)
                # Update the current cell with the minimum score
                current_table[i][j] = result

        # Perform traceback to find the aligned sequences
        self.needleman_Wunsch_Algorithm(current_table, m, n, a, b)

        # Reverse the aligned sequences to get the correct order
        self.x.reverse()
        self.y.reverse()

        # Return the alignment score and the aligned sequences
        return current_table[m][n], str(self.x), str(self.y)

    """ 
    This function performs the banded algorithm for sequence a and b
    
    @param a is the sequence a
    @param b is the sequence b
    @return results is the score of the banded alignment
    """

    def banded(self, a, b):
        self.x.clear()
        self.y.clear()

        # Set initial indices for banded alignment
        m = MAXINDELS
        n = MAXINDELS

        # Initialize the band table
        band_width = 2 * MAXINDELS + 1
        band_table = [[math.inf] * band_width for _ in range(len(a) + 1)]

        # Initialize the first row of the band table
        for i in range(1, min(len(a) + 1, MAXINDELS + 1)):
            band_table[i][MAXINDELS - i] = INDEL * i

        for i in range(MAXINDELS + 1, len(a) + 1):
            for j in range(band_width):
                band_table[i][j] = INDEL * MAXINDELS

        # Perform dynamic programming within the band
        for i in range(1, len(a) + 1):
            start_col = max(1, i - MAXINDELS)
            end_col = min(len(b), i + MAXINDELS)
            for j in range(start_col, end_col + 1):
                diff = SUB if a[i - 1] != b[j - 1] else MATCH
                is_matching = band_table[i - 1][j - i + MAXINDELS] + diff
                is_deleted = band_table[i - 1][j - i + MAXINDELS + 1] + INDEL
                is_inserted = band_table[i][j - i + MAXINDELS - 1] + INDEL
                band_table[i][j - i + MAXINDELS] = min(is_matching, is_deleted, is_inserted)

        # Perform traceback to find the aligned sequences
        self.needleman_Wunsch_Algorithm(band_table, m, n, a, b)

        # Reverse the aligned sequences to get the correct order
        self.x.reverse()
        self.y.reverse()

        # Return the alignment score and the aligned sequences
        return band_table[len(a)][MAXINDELS], str(self.x), str(self.y)

    """
    This function aligns the sequences based on whether they are unbanded or banded
    
    @param sequences is a list of the ten sequences
    @param table is a handle to the GUI so it can be updated as you find results
    @param banded is a boolean that tells you whether you should compute a banded alignment or full alignment
    @param align_length tells you how many base pairs to use in computing the alignment
    @return results is the score of the alignment 
    """

    def align(self, sequences, table, banded, align_length):
        # Set the banded flag and maximum characters to align
        self.banded = banded
        self.MaxCharactersToAlign = align_length
        results = []

        # Loop through each pair of sequences and align them
        for i in range(len(sequences)):
            jresults = []
            for j in range(len(sequences)):
                if j < i:
                    # If j < i, do not perform alignment
                    s = {}
                else:
                    # Extract sequences for alignment
                    sequencex = sequences[i][:align_length] if align_length <= len(sequences[i]) else sequences[i]
                    sequencey = sequences[j][:align_length] if align_length <= len(sequences[j]) else sequences[j]

                    ###################################################################################################
                    # your code should replace these three statements and populate the three variables: score, alignment1 and alignment2
                    # score = i + j;
                    # alignment1 = 'abc-easy  DEBUG:(seq{}, {} chars,align_len={}{})'.format(i + 1,
                    #                                                                        len(sequences[i]),
                    #                                                                        align_length,
                    #                                                                        ',BANDED' if banded else '')
                    # alignment2 = 'as-123--  DEBUG:(seq{}, {} chars,align_len={}{})'.format(j + 1,
                    #                                                                        len(sequences[j]),
                    #                                                                        align_length,
                    #                                                                        ',BANDED' if banded else '')
                    # ###################################################################################################

                    print(i, j)
                    # Perform alignment based on the banded flag
                    if banded:
                        score, alignment1, alignment2 = self.banded(sequencex, sequencey)
                    else:
                        score, alignment1, alignment2 = self.unbanded(sequencex, sequencey)
                    # score = out[0]
                    # alignment1 = out[1]
                    # alignment2 = out[2]

                    # Store alignment results
                    s = {'align_cost': score, 'seqi_first100': alignment1, 'seqj_first100': alignment2}
                    table.item(i, j).setText('{}'.format(int(score) if score != math.inf else score))
                    table.update()
                jresults.append(s)
            results.append(jresults)
        return results
