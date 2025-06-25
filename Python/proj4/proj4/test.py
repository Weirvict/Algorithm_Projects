import unittest
from GeneSequencing import GeneSequencing


class TestGeneSequencing(unittest.TestCase):
    def setUp(self):
        # Initialize GeneSequencing object
        self.gene_sequencing = GeneSequencing()

    def test_create_table_values(self):
        # Test create_table_values function for unbanded type
        table_unbanded = self.gene_sequencing.create_table_values("ATCG", "ATCG", "unbanded")
        self.assertEqual(len(table_unbanded), 5)
        self.assertEqual(len(table_unbanded[0]), 5)

        # Test create_table_values function for banded type
        table_banded = self.gene_sequencing.create_table_values("ATCG", "ATCG", "banded")
        self.assertEqual(len(table_banded), 5)
        self.assertEqual(len(table_banded[0]), 7)

    def test_needleman_Wunsch_Algorithm(self):
        # Test needleman_Wunsch_Algorithm function
        current_table = [[0 for _ in range(5)] for _ in range(5)]
        self.gene_sequencing.needleman_Wunsch_Algorithm(current_table, 4, 4, "ATCG", "ATCG")
        self.assertEqual(current_table[4][4], 0, "Expected score to be 0 for identical sequences")

    def test_unbanded(self):
        # Test unbanded function
        score, alignment1, alignment2 = self.gene_sequencing.unbanded("ATCG", "ATCG")
        self.assertEqual(score, 0, "Expected score to be 0 for identical sequences")

    def test_banded(self):
        # Test banded function
        score, alignment1, alignment2 = self.gene_sequencing.banded("ATCG", "ATCG")
        self.assertEqual(score, 0, "Expected score to be 0 for identical sequences")


if __name__ == '__main__':
    unittest.main()
