#ifndef GENERALIZE_H
#define GENERALIZE_H

#include "examples.h"
#include <assert.h>
#include <algorithm>

namespace digital_ai
{
    /// @brief A class which selects a bit by index and also states whether or not it will be inverting that bit.
    class literal
    {
    private:
        size_t m_input_index = 0;
        bool m_invert = false;

    public:
        literal(

        )
        {
            
        }

        literal(
            const size_t& a_input_index,
            const bool& a_invert
        ) :
            m_input_index(a_input_index),
            m_invert(a_invert)
        {
            
        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            // Perform an XOR in order to conditionally invert the
            // input boolean.
            return a_input[m_input_index] != m_invert;
        }

        bool operator==(
            const literal& a_literal
        ) const
        {
            return 
                m_input_index == a_literal.m_input_index &&
                m_invert      == a_literal.m_invert;
        }

    };

    /// @brief A class which acts as a boolean product of literals.
    class literal_product
    {
    private:
        std::vector<literal> m_literals;
        
    public:
        literal_product(
            const std::vector<literal>& a_literals
        ) :
            m_literals(a_literals)
        {

        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            for (literal& l_literal : m_literals)
                if (!l_literal.evaluate(a_input))
                    return false;
            return true;
        }

        const std::vector<literal>& literals(

        ) const
        {
            return m_literals;
        }

    };

    /// @brief A class which acts as a boolean sum of products of literals.
    class sum_of_products
    {
    private:
        std::vector<literal_product> m_literal_products;
        
    public:
        sum_of_products(
            const std::vector<literal_product>& a_literal_products
        ) :
            m_literal_products(a_literal_products)
        {

        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            for (literal_product& l_literal_product : m_literal_products)
                if (l_literal_product.evaluate(a_input))
                    return true;
            return false;
        }

        const std::vector<literal_product>& literal_products(

        ) const
        {
            return m_literal_products;
        }

    };

    /// @brief A class which acts as a vector of sums of products of literals.
    ///        (Used for multiple-output functions)
    class sum_of_products_string
    {
    private:
        std::vector<sum_of_products> m_binary_functions;

    public:
        sum_of_products_string(
            const std::vector<sum_of_products>& a_binary_functions
        ) :
            m_binary_functions(a_binary_functions)
        {
            
        }

        std::vector<bool> evaluate(
            const std::vector<bool>& a_input
        )
        {
            std::vector<bool> l_result(m_binary_functions.size());
            for (int i = 0; i < l_result.size(); i++)
                l_result[i] = m_binary_functions[i].evaluate(a_input);
            return l_result;
        }

    };

    /// @brief This function defines whether a given literal will cover a set of literals
    ///        which will be treated as a product.
    /// @param  
    /// @param a_literals 
    /// @return 
    bool covers(
        const literal& a_literal,
        const literal_product a_product
    )
    {
        return 
            std::find(a_product.literals().begin(), a_product.literals().end(), a_literal) != 
            a_product.literals().end();
    }

    /// @brief A function which yields a product of literals where each literal is selected under the
    ///        following conditions: the literal must be present in the satisfying input and must be absent
    ///        in the unsatisfying input.
    /// @param a_satisfying_input 
    /// @param a_unsatisfying_input 
    /// @return 
    literal_product difference_product(
        const satisfying_input&  a_satisfying_input,
        const unsatisfying_input& a_unsatisfying_input
    )
    {
        assert(a_satisfying_input.size() == a_unsatisfying_input.size());
        std::vector<literal> l_result;
        for (int i = 0; i < a_satisfying_input.size(); i++)
        {
            if (a_satisfying_input[i] != a_unsatisfying_input[i])
                l_result.push_back(literal(i, !a_satisfying_input[i]));
        }
        return literal_product(l_result);
    }

    /// @brief This function tries to get the single literal which maximally increases
    ///        the size of the coverage over the difference product domain when appended
    ///        to the current literal product "a_current_covering_literals".
    /// @param a_current_covering_literals 
    /// @param a_unsatisfying_inputs 
    /// @param a_satisfying_input 
    /// @param a_result 
    /// @return 
    bool try_get_maximally_covering_literal(
        const std::vector<literal>& a_current_covering_literals,
        const std::vector<unsatisfying_input*>& a_unsatisfying_inputs,
        const satisfying_input* a_satisfying_input,
        literal& a_result
    )
    {
        // Generate a vector of literals, for which there exist at least one difference
        // between the one-example provided and one of the zero-examples.

        std::vector<literal> l_unioned_literal_differences;
        std::vector<size_t>         l_unioned_literal_differences_counts;

        for (const unsatisfying_input* a_false_example : a_unsatisfying_inputs)
        {
            literal_product l_difference_product =
                difference_product(*a_satisfying_input, *a_false_example);
            
            for (const literal& l_literal : a_current_covering_literals)
            {
                if (covers(l_literal, l_difference_product))
                    // Check if any of the literals in the covering product
                    // service so as to cover this difference product.
                    goto do_not_union;
            }

            for (const literal& l_literal : l_difference_product.literals())
            {
                // Try to get the position of this literal within the unioned set.
                std::vector<literal>::iterator l_literal_position = 
                    std::find(
                        l_unioned_literal_differences.begin(), l_unioned_literal_differences.end(),
                        l_literal);
                
                if (l_literal_position == l_unioned_literal_differences.end())
                {
                    // Insert the literal into the unioned set.
                    l_literal_position = 
                        l_unioned_literal_differences.insert(
                        l_unioned_literal_differences.end(), l_literal);
                    
                    // Insert the count into the unioned literal count set.
                    l_unioned_literal_differences_counts.push_back(0);
                }
                
                size_t l_literal_index = l_literal_position - l_unioned_literal_differences.begin();

                // Increment the count associated with literal.
                l_unioned_literal_differences_counts[l_literal_index]++;

            }

            do_not_union:;
        }

        if (l_unioned_literal_differences.size() == 0)
            // Since no literal differences were incurred, return indicating that fact.
            return false;

        // Get the index of the largest count in all the literal occurance counts.
        size_t l_maximum_literal_occurance_count = 
            std::max_element(
                l_unioned_literal_differences_counts.begin(),
                l_unioned_literal_differences_counts.end())
            - l_unioned_literal_differences_counts.begin();

        // Set the result to be the literal associated with this count.
        a_result = l_unioned_literal_differences[l_maximum_literal_occurance_count];

        return true;

    }

    /// @brief This function acquires a covering product. This covering product acts so as to cover
    ///        one satisfying input and to do so while using few literals. None of the unsatisfying
    ///        inputs will be covered by this product.
    /// @param a_unsatisfying_inputs 
    /// @param a_satisfying_input 
    /// @return 
    literal_product cover(
        const std::vector<unsatisfying_input*>& a_unsatisfying_inputs,
        const satisfying_input* a_satisfying_input
    )
    {
        std::vector<literal> l_covering_literals;
        literal l_covering_literal;
        
        while(
            try_get_maximally_covering_literal(
                l_covering_literals,
                a_unsatisfying_inputs,
                a_satisfying_input,
                l_covering_literal
            )
        )
        {
            // Append the maximally covering literal to the list
            l_covering_literals.push_back(l_covering_literal);
        }

        return literal_product(l_covering_literals);

    }

    /// @brief Gets a sum of covering products which when coupled, will cover all satisfying inputs.
    ///        this will not cover any unsatisfying inputs. It will however cover some unresolved inputs.
    ///        This is where the "generalization" occurred.
    /// @param a_partitioned_example_set 
    /// @return 
    sum_of_products generalize(
        const partitioned_example_set& a_partitioned_example_set
    )
    {
        std::vector<literal_product> l_covering_products;
        
        for (int i = 0; i < a_partitioned_example_set.m_satisfying_inputs.size(); i++)
        {
            l_covering_products.push_back(cover(
                a_partitioned_example_set.m_unsatisfying_inputs,
                a_partitioned_example_set.m_satisfying_inputs[i]
            ));
        }

        return sum_of_products(l_covering_products);

    }

    /// @brief This will produce a vector of covering sums which each are designated to the bit output
    ///        associated with their indices.
    /// @param a_raw_examples 
    /// @return 
    sum_of_products_string generalize(
        const std::vector<raw_example>& a_raw_examples
    )
    {
        std::vector<sum_of_products> l_output_bit_functions;

        // Create all of the output_bit_example_sets
        for (int i = 0; i < a_raw_examples[0].m_output.size(); i++)
        {
            l_output_bit_functions.push_back(generalize(partitioned_example_set(a_raw_examples, i)));

        }

        return sum_of_products_string(l_output_bit_functions);

    }

}

#endif
