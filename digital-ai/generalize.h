#ifndef GENERALIZE_H
#define GENERALIZE_H

#include "examples.h"
#include <assert.h>
#include <algorithm>
#include <map>
#include <chrono>
#include <iostream>
#include <future>

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

        /// @brief This operator determines if the argued literal product is
        ///        functionally equal to the local literal product.
        /// @param a_literal_product 
        /// @return 
        bool operator==(
            const literal_product& a_literal_product
        ) const
        {
            if (m_literals.size() != a_literal_product.m_literals.size())
                return false;

            // Make sure that every local literal is in the other literal product.
            for (const literal& l_literal_0 : m_literals)
            {
                bool l_literal_is_shared = false;

                for (const literal& l_literal_1 : a_literal_product.m_literals)
                {
                    if (l_literal_1 == l_literal_0)
                    {
                        l_literal_is_shared = true;
                        break;
                    }
                }

                if (!l_literal_is_shared)
                    return false;

            }

            // Make sure that every literal in the other product is shared locally.
            for (const literal& l_literal_0 : a_literal_product.m_literals)
            {
                bool l_literal_is_shared = false;

                for (const literal& l_literal_1 : m_literals)
                {
                    if (l_literal_1 == l_literal_0)
                    {
                        l_literal_is_shared = true;
                        break;
                    }
                }

                if (!l_literal_is_shared)
                    return false;

            }

            return true;
            
        }

        bool operator!=(
            const literal_product& a_literal_product
        ) const
        {
            return !operator==(a_literal_product);
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

        /// @brief This method currently eliminates duplicate literal products.
        void simplify(

        )
        {
            for (int i = m_literal_products.size() - 1; i >= 0; i--)
            {
                auto l_literal_product_0_iterator = m_literal_products.begin() + i;
                
                bool l_repeated_literal_product = false;
                
                for (int j = 0; j < i; j++)
                {
                    auto l_literal_product_1_iterator = m_literal_products.begin() + j;

                    if (*l_literal_product_1_iterator == *l_literal_product_0_iterator)
                    {
                        l_repeated_literal_product = true;
                        break;
                    }
                }

                // Erase a single occurance of the repeated literal product.
                if (l_repeated_literal_product)
                {
                    m_literal_products.erase(l_literal_product_0_iterator);
                }

            }

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

        void simplify(

        )
        {
            for (auto& l_sum : m_binary_functions)
                l_sum.simplify();
        }

        const std::vector<sum_of_products>& sums_of_products(

        ) const
        {
            return m_binary_functions;
        }

    };

    /// @brief This function returns the element-wise xor of the two argued
    ///        bit-strings.
    /// @param a_x0 
    /// @param a_x1 
    /// @return 
    std::vector<bool> bit_string_xor(
        const std::vector<bool>& a_x0,
        const std::vector<bool>& a_x1
    )
    {
        assert(a_x0.size() == a_x1.size());
        std::vector<bool> l_result(a_x0.size());
        for (int i = 0; i < l_result.size(); i++)
            l_result[i] = a_x0[i] != a_x1[i];
        return l_result;
    }

    /// @brief This function tries to get the single literal which restricts coverage maximally
    ///        over the unsatisfying inputs.
    /// @param a_current_covering_literals
    /// @param a_unsatisfying_inputs
    /// @param a_satisfying_input 
    /// @param a_result 
    /// @return
    bool try_get_maximally_covering_literal(
        const std::vector<std::vector<bool>>& a_input_xors,
        const std::vector<bool>& a_input_xors_checked,
        size_t& a_result
    )
    {
        // Make a vector of the number of occurrances of a bit's xor being a 1.
        std::vector<size_t> l_bit_xor_enable_counts(a_input_xors[0].size());

        for (int i = 0; i < a_input_xors.size(); i++)
        {
            if (a_input_xors_checked[i])
                continue;
            
            for (int j = 0; j < l_bit_xor_enable_counts.size(); j++)
                l_bit_xor_enable_counts[j] += a_input_xors[i][j];

        }

        a_result = 
            std::max_element(l_bit_xor_enable_counts.begin(), l_bit_xor_enable_counts.end()) -
            l_bit_xor_enable_counts.begin();

        // If the maximum occuring literal's occurrance was zero, just return false.
        if (l_bit_xor_enable_counts[a_result] == 0)
            return false;

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
        std::vector<std::vector<bool>> l_input_xors(a_unsatisfying_inputs.size());
        std::vector<bool>              l_input_xors_checked(a_unsatisfying_inputs.size());

        // Precompute all literal differences before trying to generalize
        for (int i = 0; i < l_input_xors.size(); i++)
            l_input_xors[i] = bit_string_xor(*a_satisfying_input, *a_unsatisfying_inputs[i]);
        
        std::vector<literal> l_covering_literals;
        size_t l_selected_literal_index = 0;

        //auto l_start = std::chrono::high_resolution_clock::now();

        while(
            try_get_maximally_covering_literal(
                l_input_xors,
                l_input_xors_checked,
                l_selected_literal_index
            )
        )
        {
            // Append the maximally covering literal to the list
            l_covering_literals.push_back(literal(l_selected_literal_index, !a_satisfying_input->at(l_selected_literal_index)));

            // Roll through, indicating which input xor's have been checked already.
            for (int i = 0; i < l_input_xors.size(); i++)
            {
                if (l_input_xors[i][l_selected_literal_index] == 1)
                    l_input_xors_checked[i] = true;
            }

        }

        //auto l_end = std::chrono::high_resolution_clock::now();

        //auto l_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(l_end - l_start);

        //std::cout << l_microseconds.count() << std::endl;

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
        std::vector<std::future<sum_of_products>> l_async_output_bit_functions;

        // Create all of the output_bit_example_sets
        for (int i = 0; i < a_raw_examples[0].m_output.size(); i++)
        {
            // Do work asynchronously so as to reduce compute time.
            l_async_output_bit_functions.push_back(
                std::async(std::launch::async, [&, i]{ return generalize(partitioned_example_set(a_raw_examples, i)); }));
        }

        std::vector<sum_of_products> l_output_bit_functions;
        
        // Resolve all asynchronous outputs.
        for (int i = 0; i < l_async_output_bit_functions.size(); i++)
            l_output_bit_functions.push_back(l_async_output_bit_functions[i].get());

        return sum_of_products_string(l_output_bit_functions);

    }

}

#endif
