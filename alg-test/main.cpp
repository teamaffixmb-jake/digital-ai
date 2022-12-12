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

void test_difference_product(

)
{
    {
        digital_ai::satisfying_input   l_satisfying_input({0, 1, 1, 0});
        digital_ai::unsatisfying_input l_unsatisfying_input({0, 1, 1, 1});

        digital_ai::literal_product l_difference_product =
            digital_ai::difference_product(l_satisfying_input, l_unsatisfying_input);

        assert(l_difference_product.literals().size() == 1);
        assert(l_difference_product.literals()[0] == digital_ai::literal(3, true));
    }

    {
        digital_ai::satisfying_input   l_satisfying_input({0, 1, 1, 0});
        digital_ai::unsatisfying_input l_unsatisfying_input({0, 0, 1, 1});

        digital_ai::literal_product l_literal_difference =
            digital_ai::difference_product(l_satisfying_input, l_unsatisfying_input);

        assert(l_literal_difference.literals().size() == 2);
        assert(l_literal_difference.literals()[0] == digital_ai::literal(1, false));
        assert(l_literal_difference.literals()[1] == digital_ai::literal(3, true));
    }

}

void test_try_get_maximally_covering_literal(

)
{
    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 0, 1, 0}, {1, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[0], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(0, false));

    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 0, 0, 1}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 0, 1, 0}, {1, 1, 0}},
            {{0, 0, 1, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[0], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(2, true));

    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 0, 0, 1}, {1, 1, 0}},
            {{0, 0, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 0, 1, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[1], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(3, true));

    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 1, 0, 0}, {0, 0, 0}},
            {{0, 1, 0, 1}, {0, 0, 1}},
            {{0, 1, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 1, 0, 1}, {1, 1, 0}},
            {{0, 0, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        digital_ai::literal l_maximally_covering_literal;

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        bool l_success = digital_ai::try_get_maximally_covering_literal({},
            l_partitioned_example_set.m_unsatisfying_inputs,
            l_partitioned_example_set.m_satisfying_inputs[1], l_maximally_covering_literal);
    
        assert(l_success);
        assert(l_maximally_covering_literal == digital_ai::literal(1, true));

    }

}

void test_cover(

)
{
    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 0, 1, 0}, {1, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(0, false), digital_ai::literal(2, true) }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, 
            {{1, 0, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[1]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(3, true), digital_ai::literal(0, false) }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 1}, {1, 1, 0}}, 
            {{1, 0, 1, 0}, {1, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[2]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(3, true), digital_ai::literal(0, false) }));
    
    }

    {
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 0}, {0, 1, 0}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 0, 0}, {0, 1, 0}},
            {{0, 1, 0, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
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

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == std::vector({ digital_ai::literal(3, true), digital_ai::literal(1, false), digital_ai::literal(2, false) }));
    
    }

    {
        // In this example, the only position in which there is a satisfying input is when minterm 0'123' is satisfied.
        // Therefore, what we derive should equal this minterm. No generalization can take place here.
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 0}, {0, 1, 0}},
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 0, 0}, {0, 1, 0}},
            {{0, 1, 0, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 0}, {0, 1, 0}},
            {{1, 0, 0, 1}, {0, 1, 0}},
            {{1, 0, 1, 0}, {0, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 0, 0}, {0, 1, 0}},
            {{1, 1, 0, 1}, {0, 1, 1}},
            {{1, 1, 1, 0}, {0, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 0);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == 
            std::vector({ 
                digital_ai::literal(1, false),
                digital_ai::literal(2, false),
                digital_ai::literal(3, true),
                digital_ai::literal(0, true) }));
    
    }

    {
        // In this example, the only position in which there is a satisfying input is when minterm 0'123' is satisfied.
        // Therefore, what we derive should equal this minterm. No generalization can take place here.
        std::vector<digital_ai::raw_example> l_raw_examples =
        {
            {{0, 0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0, 1}, {0, 0, 1}},
            {{0, 0, 1, 0}, {0, 1, 0}}, // Using this one as our satisfying example of focus
            {{0, 0, 1, 1}, {0, 1, 1}},
            {{0, 1, 0, 0}, {0, 1, 0}},
            {{0, 1, 0, 1}, {0, 1, 0}},
            {{0, 1, 1, 0}, {1, 1, 0}},
            {{0, 1, 1, 1}, {0, 1, 0}},
            {{1, 0, 0, 0}, {0, 1, 0}},
            {{1, 0, 0, 1}, {0, 1, 0}},
            {{1, 0, 1, 0}, {0, 1, 0}},
            {{1, 0, 1, 1}, {0, 1, 0}},
            {{1, 1, 0, 0}, {0, 1, 0}},
            {{1, 1, 0, 1}, {0, 1, 1}},
            {{1, 1, 1, 0}, {0, 1, 0}},
            {{1, 1, 1, 1}, {0, 1, 0}},
        };

        digital_ai::partitioned_example_set l_partitioned_example_set(l_raw_examples, 1);

        // This call only takes into account a SINGLE satisfying input and ALL unsatisfying inputs.
        digital_ai::literal_product l_covering_product = digital_ai::cover(
            l_partitioned_example_set.m_unsatisfying_inputs,l_partitioned_example_set.m_satisfying_inputs[0]);
    
        assert(l_covering_product.literals() == 
            std::vector({ 
                digital_ai::literal(2, false) }));
    
    }

}

void unit_test_main(

)
{
    test_data_partitioning();
    test_difference_product();
    test_try_get_maximally_covering_literal();
    test_cover();
}

int main(

)
{
    unit_test_main();

    return 0;
}
