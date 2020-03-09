#include <algorithm>
#include <cassert>
#include <cstdio>
#include <set>
#include <vector>

using namespace std;

template < typename T >
struct Field
{
	T values[81];
};
struct Position
{
	bool candidates[9];
};
Field< Position > convert(Field< int > const &field)
{
	Field< Position > retval;

	transform(
		  begin(field.values)
		, end(field.values)
		, begin(retval.values)
		, [](int value){
				Position position;
				assert(value <= 9);
				switch (value)
				{
				case 0 :
				{
					bool const candidates[] = { true, true, true, true, true, true, true, true, true };
					copy(begin(candidates), end(candidates), begin(position.candidates));
					break;
				}
				default :
					bool const candidates[] = { false, false, false, false, false, false, false, false, false };
					copy(begin(candidates), end(candidates), begin(position.candidates));
					position.candidates[value - 1] = true;
					break;
				}
				return position;
			}
		);
	return retval;
}
int convert(Position position)
{
	if (count(begin(position.candidates), end(position.candidates), true) == 1)
	{
		return distance(begin(position.candidates), find(begin(position.candidates), end(position.candidates), true)) + 1;
	}
	else
	{
		return 0;
	}
}
Field< int > convert(Field< Position > const &field)
{
	Field< int > retval;
	
	transform(
		  begin(field.values)
		, end(field.values)
		, begin(retval.values)
		, [](Position position){ return convert(position); }
		);
	
	return retval;
}
void printField(FILE *out, Field< int > const &field)
{
	fprintf(out,
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		  "\t%d %d %d   %d %d %d   %d %d %d\n"
		, field.values[ 0], field.values[ 1], field.values[ 2], field.values[ 3], field.values[ 4], field.values[ 5], field.values[ 6], field.values[ 7], field.values[ 8]
		, field.values[ 9], field.values[10], field.values[11], field.values[12], field.values[13], field.values[14], field.values[15], field.values[16], field.values[17]
		, field.values[18], field.values[19], field.values[20], field.values[21], field.values[22], field.values[23], field.values[24], field.values[25], field.values[26]
		, field.values[27], field.values[28], field.values[29], field.values[30], field.values[31], field.values[32], field.values[33], field.values[34], field.values[35]
		, field.values[36], field.values[37], field.values[38], field.values[39], field.values[40], field.values[41], field.values[42], field.values[43], field.values[44]
		, field.values[45], field.values[46], field.values[47], field.values[48], field.values[49], field.values[50], field.values[51], field.values[52], field.values[53]
		, field.values[54], field.values[55], field.values[56], field.values[57], field.values[58], field.values[59], field.values[60], field.values[61], field.values[62]
		, field.values[63], field.values[64], field.values[65], field.values[66], field.values[67], field.values[68], field.values[69], field.values[70], field.values[71]
		, field.values[72], field.values[73], field.values[74], field.values[75], field.values[76], field.values[77], field.values[78], field.values[79], field.values[80]
		);
}
void printField(FILE *out, Field< Position > const &field)
{
	printField(out, convert(field));
}
// return all indices in the same row as index, excluding index itself
vector< int > getRow(int const index)
{
	int curr_index((index / 9) * 9);
	vector< int > retval(9);
	generate(retval.begin(), retval.end(), [&](){ return curr_index++; });
	retval.erase(retval.begin() + (index % 9));
	
	assert(retval.size() == 8);
	
	return retval;
}
// return all indices in the same column as index, excluding index itself
vector< int > getColumn(int const index)
{
	int curr_index(index % 9);
	vector< int > retval(9);
	generate(retval.begin(), retval.end(), [&](){ int retval(curr_index); curr_index += 9; return retval; });
	retval.erase(retval.begin() + (index / 9));
	
	assert(retval.size() == 8);
	
	return retval;
}
// return all indices in the same cell as index, excluding index itself
vector< int > getCell(int const index, bool include_seed = false)
{
	// we'll brute-force it
	vector< int > retval;
	for (int i(0); i < 81; ++i)
	{
		if ((include_seed || (i != index)) && (((i / 3) % 3) == ((index / 3) % 3)) && ((i / 27) == (index / 27)))
		{
			retval.push_back(i);
		}
		else
		{ /* not this one */ }
	}
	assert(retval.size() == (include_seed ? 9 : 8));

	return retval;
}
bool solved(Position const &position)
{
	return count(begin(position.candidates), end(position.candidates), true) == 1;
}

bool solved(Field< Position > const &field)
{
	/* there are two ways to implement this function: either we convert the field to a Field< int > and call solved on
	 * that (the lazy way), or we iterate through our values and check whether any of them remain unsolved (the 
	 * somewhat more elegant way). We'll go for the second option. */
	 for (auto position : field.values)
	 {
	 	if (!solved(position)) return false;
	 }
	 return true;
}
bool solved(Field< int > const &field)
{
	return count(begin(field.values), end(field.values), 0) == 0;
}

Field< Position > simpleSolve(Field< Position > field)
{
	for (int i(0); i < 81; ++i)
	{
		if (solved(field.values[i])) continue;
		auto row_siblings(getRow(i));
		auto column_siblings(getColumn(i));
		auto cell_siblings(getCell(i));
		set< int > siblings;
		siblings.insert(row_siblings.begin(), row_siblings.end());
		siblings.insert(column_siblings.begin(), column_siblings.end());
		siblings.insert(cell_siblings.begin(), cell_siblings.end());
		for (auto sibling : siblings)
		{
			if (solved(field.values[sibling]))
			{
				field.values[i].candidates[convert(field.values[sibling]) - 1] = false;
			}
		}
	}
	return field;
}
vector< Position > getValues(Field< Position > const &field, vector< int > const &indices)
{
	vector< Position > retval;
	for(auto index : indices)
	{
		retval.push_back(field.values[index]);
	}
	return retval;
}
bool sameRow(int lhs, int rhs)
{
	return ((lhs / 9) == (rhs / 9));
}
bool sameColumn(int lhs, int rhs)
{
	return ((lhs % 9) == (rhs % 9));
}
bool sameCell(int lhs, int rhs)
{
	return ((lhs / 3) % 3) == ((rhs / 3) % 3)
		&& (lhs / 27) == (rhs / 27)
		;
}
Field< Position > promote_(Field< Position > field, int index, int value)
{
	for (int i(0); i < 9; ++i)
	{
		field.values[index].candidates[i] = (i == value);
	}
	return field;
}
Field< Position > promote(Field< Position > field, int cell, int value)
{
	auto cellmates(getCell(cell, true));
	auto values(getValues(field, cellmates));
	vector< int > positions_with_value_as_candidate;
	assert(cellmates.size() == 9);
	assert(values.size() == 9);
	for (int i(0); i < 9; ++i)
	{
		if (values[i].candidates[value])
		{
			positions_with_value_as_candidate.push_back(i);
		}
		else
		{ /* not a candidate here */ }
	}
	if (positions_with_value_as_candidate.size() == 1)
	{
		auto the_index(cellmates[positions_with_value_as_candidate[0]]);
		field = promote_(field, the_index, value);
	}
	else
	{ /* can't promote this value */ }
	return field;
}
Field< Position > promote(Field< Position > field, int cell)
{
	for (int i(0); i < 9; ++i)
	{
		field = promote(field, cell, i);
	}
	return field;
}
Field< Position > promote(Field< Position > field)
{
	for (int i : {0, 3, 6, 27, 30, 33, 54, 57, 60})
	{
		field = promote(field, i);
	}
	return field;
}
Field< Position > reduce(Field< Position > field, int cell, int value)
{
	auto cellmates(getCell(cell, true));
	auto values(getValues(field, cellmates));
	vector< int > positions_with_value_as_candidate;
	assert(cellmates.size() == 9);
	assert(values.size() == 9);
	for (int i(0); i < 9; ++i)
	{
		if (values[i].candidates[value])
		{
			positions_with_value_as_candidate.push_back(cellmates[i]);
		}
		else
		{ /* not a candidate here */ }
	}
	assert(!positions_with_value_as_candidate.empty());
	
	bool all_same_row(true);
	for_each(
		  positions_with_value_as_candidate.begin() + 1
		, positions_with_value_as_candidate.end()
		, [&](int position){ all_same_row &= sameRow(positions_with_value_as_candidate.front(), position); }
		);
	if (all_same_row)
	{
		auto entire_row(getRow(positions_with_value_as_candidate.front()));
		decltype(entire_row) row_outside_cell;
		copy_if(
			  entire_row.begin()
			, entire_row.end()
			, back_inserter(row_outside_cell)
			, [=](int position){ return !sameCell(positions_with_value_as_candidate.front(), position); }
			);
		for_each(
			  row_outside_cell.begin()
			, row_outside_cell.end()
			, [&](int position){ field.values[position].candidates[value] = false; }
			);
	}
	else
	{ /* not all in the same row */ }
	
	bool all_same_column(true);
	for_each(
		  positions_with_value_as_candidate.begin() + 1
		, positions_with_value_as_candidate.end()
		, [&](int position){ all_same_column &= sameColumn(positions_with_value_as_candidate.front(), position); }
		);
	if (all_same_column)
	{
		auto entire_column(getColumn(positions_with_value_as_candidate.front()));
		decltype(entire_column) column_outside_cell;
		copy_if(
			  entire_column.begin()
			, entire_column.end()
			, back_inserter(column_outside_cell)
			, [=](int position){ return !sameCell(positions_with_value_as_candidate.front(), position); }
			);
		for_each(
			  column_outside_cell.begin()
			, column_outside_cell.end()
			, [&](int position){ field.values[position].candidates[value] = false; }
			);
	}
	else
	{ /* not all in the same column */ }
	
	return field;
}
Field< Position > reduce(Field< Position > field, int cell)
{
	for (int i(0); i < 9; ++i)
	{
		field = reduce(field, cell, i);
	}
	return field;
}
Field< Position > reduce(Field< Position > field)
{
	for (int i : {0, 3, 6, 27, 30, 33, 54, 57, 60})
	{
		field = reduce(field, i);
	}
	return field;
}
Field< Position > solve(Field< Position > field)
{
unsigned int step(1);
	// we will continue to try to solve this as long as we're making progress and haven't completely solved it
	while (!solved(field))
	{
		auto const solved_at_start(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
		while (!solved(field))
		{
			auto const prev_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
			field = simpleSolve(field);
			auto const curr_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
			if (prev_solved == curr_solved) break; // no longer making progress
		}
		while (!solved(field))
		{
			auto const prev_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
			field = promote(field);
			auto const curr_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
			if (prev_solved == curr_solved) break; // no longer making progress
		}
		while (!solved(field))
		{
			auto const prev_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
			field = reduce(field);
			auto const curr_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
			if (prev_solved == curr_solved) break; // no longer making progress
		}
		auto const curr_solved(count_if(begin(field.values), end(field.values), [](Position position){ return solved(position); }));
		if (solved_at_start == curr_solved) break; // no longer making progress
	}
	return field;
}
Field< int > solve(Field< int > const &field)
{
	return convert(solve(convert(field)));
}

int example[] = {
#if 0
	  5, 3, 0,  0, 7, 0,  0, 0, 0
	, 6, 0, 0,  1, 9, 5,  0, 0, 0
	, 0, 9, 8,  0, 0, 0,  0, 6, 0
	
	, 8, 0, 0,  0, 6, 0,  0, 0, 3
	, 4, 0, 0,  8, 0, 3,  0, 0, 1
	, 7, 0, 0,  0, 2, 0,  0, 0, 6
	
	, 0, 6, 0,  0, 0, 0,  2, 8, 0
	, 0, 0, 0,  4, 1, 9,  0, 0, 5
	, 0, 0, 0,  0, 8, 0,  0, 7, 9
#endif
#if 0
	  9, 4, 3,  0, 8, 2,  1, 6, 0
	, 0, 1, 6,  0, 4, 5,  9, 8, 2
	, 2, 0, 0,  1, 0, 6,  3, 4, 7
	
	, 0, 8, 5,  0, 0, 3,  0, 9, 0
	, 0, 0, 9,  0, 0, 7,  0, 5, 0
	, 0, 3, 0,  8, 0, 0,  0, 2, 6
	
	, 8, 0, 0,  5, 2, 0,  6, 0, 0
	, 0, 0, 0,  0, 0, 0,  5, 0, 8
	, 5, 0, 0,  6, 0, 8,  2, 0, 0
#endif
#if 0
	  0, 0, 0,  0, 9, 0,  1, 2, 5
	, 0, 1, 0,  5, 6, 2,  0, 0, 9
	, 0, 5, 0,  0, 0, 0,  4, 6, 0
	
	, 7, 0, 0,  0, 4, 6,  0, 0, 0
	, 5, 0, 2,  1, 8, 7,  9, 0, 3
	, 9, 0, 8,  2, 3, 5,  6, 1, 0
	
	, 0, 0, 5,  0, 0, 0,  3, 7, 0
	, 6, 2, 0,  0, 1, 3,  0, 0, 0
	, 1, 0, 3,  7, 0, 0,  0, 0, 0
#endif
	  8, 0, 2,  0, 0, 0,  0, 0, 3
	, 0, 5, 0,  1, 0, 0,  9, 0, 0
	, 0, 0, 0,  0, 0, 8,  4, 0, 0
	
	, 0, 0, 0,  0, 0, 0,  0, 0, 0
	, 0, 0, 8,  0, 0, 0,  0, 2, 6
	, 1, 7, 0,  0, 0, 0,  5, 0, 0
	
	, 0, 0, 0,  0, 0, 4,  1, 0, 0
	, 0, 6, 9,  0, 8, 0,  0, 0, 7
	, 3, 0, 4,  7, 6, 0,  0, 0, 9
	};
int main()
{
	Field< int > input_field;
	copy(begin(example), end(example), begin(input_field.values));
	Field< int > field;
	copy(begin(example), end(example), begin(field.values));
	printf("input:\n");
	printField(stdout, input_field);
	field = solve(field);
	printf("The solver %s the puzzle!\n", solved(field) ? "solved" : "failed to solve");
	printf("output:\n");
	printField(stdout, field);
}
