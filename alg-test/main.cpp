#include "digital-ai/generalize.h"
#include <iostream>
#include <assert.h>

void test_data_partitioning(

)
{
    std::vector<digital_ai::raw_example> l_raw_examples =
    {
        {{0, 0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0, 1}, {0, 0, 1}},
        {{0, 0, 1, 0}, {0, 1, 0}},
        {{0, 0, 1, 1}, {0, 1, 1}},
        {{0, 1, 0, 0}, {0, 1, 0}},
        {{0, 1, 0, 1}, {0, 1, 0}},
        {{0, 1, 1, 0}, {1, 1, 0}},
        {{0, 1, 1, 1}, {0, 1, 0}},
        {{1, 0, 0, 0}, {1, 1, 0}},
        {{1, 0, 0, 1}, {1, 1, 0}},
        {{1, 0, 1, 0}, {1, 1, 0}},
        {{1, 0, 1, 1}, {0, 1, 0}},
        {{1, 1, 0, 0}, {1, 1, 0}},
        {{1, 1, 0, 1}, {1, 1, 1}},
        {{1, 1, 1, 0}, {1, 1, 0}},
        {{1, 1, 1, 1}, {0, 1, 0}},
    };

    {
        auto l_first_satisfying = &l_raw_examples[6].m_input;
        auto l_last_satisfying = &l_raw_examples[14].m_input;

        auto l_first_unsatisfying = &l_raw_examples[0].m_input;
        auto l_last_unsatisfying = &l_raw_examples[15].m_input;

        digital_ai::partitioned_example_set l_partitioned_example_set(
            l_raw_examples,
            0
        );
        
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.front() == l_first_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.back() == l_last_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.front() == l_first_unsatisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.back() == l_last_unsatisfying);

    }

    {
        auto l_first_satisfying = &l_raw_examples[2].m_input;
        auto l_last_satisfying = &l_raw_examples[15].m_input;

        auto l_first_unsatisfying = &l_raw_examples[0].m_input;
        auto l_last_unsatisfying = &l_raw_examples[1].m_input;

        digital_ai::partitioned_example_set l_partitioned_example_set(
            l_raw_examples,
            1
        );
        
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.front() == l_first_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.back() == l_last_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.front() == l_first_unsatisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.back() == l_last_unsatisfying);

    }

    {
        auto l_first_satisfying = &l_raw_examples[1].m_input;
        auto l_last_satisfying = &l_raw_examples[13].m_input;

        auto l_first_unsatisfying = &l_raw_examples[0].m_input;
        auto l_last_unsatisfying = &l_raw_examples[15].m_input;

        digital_ai::partitioned_example_set l_partitioned_example_set(
            l_raw_examples,
            2
        );
        
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.front() == l_first_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_satisfying_inputs.back() == l_last_satisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.front() == l_first_unsatisfying);
        assert((std::vector<bool>*)l_partitioned_example_set.m_unsatisfying_inputs.back() == l_last_unsatisfying);

    }

}

void test_literal_difference(

)
{
    {
        digital_ai::satisfying_input   l_satisfying_input({0, 1, 1, 0});
        digital_ai::unsatisfying_input l_unsatisfying_input({0, 1, 1, 1});

        std::vector<digital_ai::literal> l_literal_difference =
            digital_ai::literal_difference(l_satisfying_input, l_unsatisfying_input);

        assert(l_literal_difference.size() == 1);
        assert(l_literal_difference[0] == digital_ai::literal(3, true));
    }

    {
        digital_ai::satisfying_input   l_satisfying_input({0, 1, 1, 0});
        digital_ai::unsatisfying_input l_unsatisfying_input({0, 0, 1, 1});

        std::vector<digital_ai::literal> l_literal_difference =
            digital_ai::literal_difference(l_satisfying_input, l_unsatisfying_input);

        assert(l_literal_difference.size() == 2);
        assert(l_literal_difference[0] == digital_ai::literal(1, false));
        assert(l_literal_difference[1] == digital_ai::literal(3, true));
    }


}

void unit_test_main(

)
{
    test_data_partitioning();
    test_literal_difference();
}

int main(

)
{
    unit_test_main();

    return 0;
}
