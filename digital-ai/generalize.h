#ifndef GENERALIZE_H
#define GENERALIZE_H

#include "examples.h"
#include <assert.h>
#include <algorithm>
#include <map>
#include <chrono>
#include <iostream>
#include <future>
#include <set>
#include <math.h>

namespace digital_ai
{
    /// @brief A class which selects a bit by index and also states whether or not it will be inverting that bit.
    class literal
    {
    private:
        size_t m_index = 0;
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
            m_index(a_input_index),
            m_invert(a_invert)
        {
            
        }

        bool evaluate(
            const std::vector<bool>& a_input
        )
        {
            // Perform an XOR in order to conditionally invert the
            // input boolean.
            return a_input[m_index] != m_invert;
        }

        bool operator==(
            const literal& a_literal
        ) const
        {
            return 
                m_index == a_literal.m_index &&
                m_invert      == a_literal.m_invert;
        }

        const size_t& index(

        ) const
        {
            return m_index;
        }

        const bool& invert(

        ) const
        {
            return m_invert;
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

        literal_product(
            const std::vector<bool>& a_state
        )
        {
            for (int i = 0; i < a_state.size(); i++)
                m_literals.push_back(literal(i, !a_state[i]));
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

    template<typename RETURN_TYPE, typename INPUT_TYPE>
    class cache
    {
    private:
        std::function<RETURN_TYPE(const INPUT_TYPE&)> m_function;
        std::unordered_map<INPUT_TYPE*, RETURN_TYPE*> m_map;
        size_t m_maximum_size = 0;

    public:
        virtual ~cache(

        )
        {
            for (auto l_it = m_map.begin(); l_it != m_map.end(); l_it++)
            {
                delete l_it->first;
                delete l_it->second;
            }
        }

        cache(
            const std::function<RETURN_TYPE(const INPUT_TYPE&)>& a_function,
            const size_t& a_size
        ) :
            m_function(a_function),
            m_maximum_size(a_size)
        {
            
        }

        RETURN_TYPE evaluate(
            const INPUT_TYPE& a_input
        )
        {
            auto l_position = std::find_if(m_map.begin(), m_map.end(),
                [&](std::unordered_map<INPUT_TYPE*, RETURN_TYPE*>::value_type l_value)
                {
                    return *l_value.first == a_input;
                });
            
            if (l_position == m_map.end())
            {
                // The cached (input, output) pair could not be found.
                
                if (m_map.size() >= m_maximum_size)
                {
                    // Erase the oldest cache to have been accessed
                    delete m_map.begin()->first;
                    delete m_map.begin()->second;
                    m_map.erase(m_map.begin());
                }

                // Compute the result manually using the function
                RETURN_TYPE l_result = m_function(a_input);

                // Insert the result into the end of the map, to signify that it was
                // the most recently accessed resource from the cache.  
                m_map.emplace(new INPUT_TYPE(a_input), new RETURN_TYPE(l_result));

                return l_result;

            }

            INPUT_TYPE*  l_dynamically_allocated_input = l_position->first;
            RETURN_TYPE* l_dynamically_allocated_result = l_position->second;

            // Erase the element at whatever position it was at
            m_map.erase(l_position);

            // Make the element at the end so as to indicate
            // that it was accessed most recently.
            m_map.emplace(l_dynamically_allocated_input, l_dynamically_allocated_result);

            return *l_dynamically_allocated_result;

        }
    
    };

    // /// @brief This function returns the element-wise xor of the two argued
    // ///        bit-strings.
    // /// @param a_x0 
    // /// @param a_x1 
    // /// @return 
    // std::vector<bool> bit_string_xor(
    //     const std::vector<bool>& a_x0,
    //     const std::vector<bool>& a_x1
    // )
    // {
    //     assert(a_x0.size() == a_x1.size());
    //     std::vector<bool> l_result(a_x0.size());
    //     for (int i = 0; i < l_result.size(); i++)
    //         l_result[i] = a_x0[i] != a_x1[i];
    //     return l_result;
    // }

    // /// @brief This function tries to get the single literal which restricts coverage maximally
    // ///        over the unsatisfying inputs.
    // /// @param a_current_covering_literals
    // /// @param a_unsatisfying_inputs
    // /// @param a_satisfying_input 
    // /// @param a_result 
    // /// @return
    // bool try_get_maximally_covering_literal(
    //     const std::vector<std::vector<bool>>& a_input_xors,
    //     const std::vector<bool>& a_input_xors_checked,
    //     size_t& a_result
    // )
    // {
    //     // Make a vector of the number of occurrances of a bit's xor being a 1.
    //     std::vector<size_t> l_bit_xor_enable_counts(a_input_xors[0].size());

    //     for (int i = 0; i < a_input_xors.size(); i++)
    //     {
    //         if (a_input_xors_checked[i])
    //             continue;
            
    //         for (int j = 0; j < l_bit_xor_enable_counts.size(); j++)
    //             l_bit_xor_enable_counts[j] += a_input_xors[i][j];

    //     }

    //     a_result = 
    //         std::max_element(l_bit_xor_enable_counts.begin(), l_bit_xor_enable_counts.end()) -
    //         l_bit_xor_enable_counts.begin();

    //     // If the maximum occuring literal's occurrance was zero, just return false.
    //     if (l_bit_xor_enable_counts[a_result] == 0)
    //         return false;

    //     return true;

    // }

    struct input_specific_unsatisfying_coverage
    {
        std::vector<unsatisfying_input*> m_enabled_coverage;
        std::vector<unsatisfying_input*> m_disabled_coverage;
    };

    std::vector<input_specific_unsatisfying_coverage> potential_unsatisfying_coverage(
        const std::vector<unsatisfying_input*>& a_covered_unsatisfying_inputs
    )
    {
        // Create the "what-if" literal coverage vector which describes the
        // unsatisfying inputs that will be covered given the selection of any given literal
        // and the current covering product's literals.
        std::vector<input_specific_unsatisfying_coverage> l_result(a_covered_unsatisfying_inputs.front()->size());
        
        for (unsatisfying_input* l_unsatisfying_input : a_covered_unsatisfying_inputs)
        {
            for (int i = 0; i < l_unsatisfying_input->size(); i++)
            {
                if (l_unsatisfying_input->at(i))
                    // Make a note that this unsatisfying input contains the non-inverted literal.
                    l_result[i].m_enabled_coverage.push_back(l_unsatisfying_input);
                else
                    // Make a note that this unsatisfying input contains the inverted literal.
                    l_result[i].m_disabled_coverage.push_back(l_unsatisfying_input);
            }
        }

        return l_result;

    }

    /// @brief This function acquires a covering product. This covering product acts so as to cover
    ///        one satisfying input and to do so while using few literals. None of the unsatisfying
    ///        inputs will be covered by this product.
    /// @param a_unsatisfying_inputs 
    /// @param a_satisfying_input 
    /// @return 
    literal_product cover(
        digital_ai::cache<std::vector<digital_ai::input_specific_unsatisfying_coverage>, 
            std::vector<digital_ai::unsatisfying_input*>>& a_cache,
        const std::vector<unsatisfying_input*>& a_covered_unsatisfying_inputs,
        const satisfying_input* a_satisfying_input
    )
    {
        //auto l_start_time = std::chrono::high_resolution_clock::now();

        std::vector<unsatisfying_input*> l_covered_unsatisfying_inputs = a_covered_unsatisfying_inputs;

        std::vector<literal> l_literals;
        
        while(l_covered_unsatisfying_inputs.size() > 0)
        {
            std::vector<input_specific_unsatisfying_coverage> l_potential_unsatisfying_coverage =
                a_cache.evaluate(l_covered_unsatisfying_inputs);
            
            literal l_minimally_covering_literal;
            size_t l_unsatisfying_coverage = SIZE_MAX;

            for (int i = 0; i < a_satisfying_input->size(); i++)
            {
                // Get the literal which minimally covers the unsatisfying part of the domain.
                if (a_satisfying_input->at(i))
                {
                    // Check enabled literal
                    if (l_potential_unsatisfying_coverage[i].m_enabled_coverage.size() < l_unsatisfying_coverage)
                    {
                        l_minimally_covering_literal = literal(i, false);
                        l_unsatisfying_coverage = l_potential_unsatisfying_coverage[i].m_enabled_coverage.size();
                    }
                }
                else
                {
                    // Check disabled literal
                    if (l_potential_unsatisfying_coverage[i].m_disabled_coverage.size() < l_unsatisfying_coverage)
                    {
                        l_minimally_covering_literal = literal(i, true);
                        l_unsatisfying_coverage = l_potential_unsatisfying_coverage[i].m_disabled_coverage.size();
                    }
                }

            }

            l_literals.push_back(l_minimally_covering_literal);

            if (l_minimally_covering_literal.invert())
            {
                l_covered_unsatisfying_inputs =
                    l_potential_unsatisfying_coverage[l_minimally_covering_literal.index()].m_disabled_coverage;
            }
            else
            {
                l_covered_unsatisfying_inputs =
                    l_potential_unsatisfying_coverage[l_minimally_covering_literal.index()].m_enabled_coverage;
            }

        }

        //auto l_end_time = std::chrono::high_resolution_clock::now();

        //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(l_end_time - l_start_time).count() << std::endl;

        return literal_product(l_literals);

    }

    /// @brief Gets a sum of covering products which when coupled, will cover all satisfying inputs.
    ///        this will not cover any unsatisfying inputs. It will however cover some unresolved inputs.
    ///        This is where the "generalization" occurred.
    /// @param a_partitioned_example_set 
    /// @return 
    sum_of_products generalize(
        digital_ai::cache<std::vector<digital_ai::input_specific_unsatisfying_coverage>, 
            std::vector<digital_ai::unsatisfying_input*>>& a_cache,
        const partitioned_example_set& a_partitioned_example_set
    )
    {
        std::vector<literal_product> l_covering_products;
        
        for (int i = 0; i < a_partitioned_example_set.m_satisfying_inputs.size(); i++)
        {
            l_covering_products.push_back(cover(
                a_cache,
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
        digital_ai::cache<std::vector<digital_ai::input_specific_unsatisfying_coverage>, 
            std::vector<digital_ai::unsatisfying_input*>>& a_cache,
        const std::vector<raw_example>& a_raw_examples
    )
    {
        // std::vector<std::future<sum_of_products>> l_async_output_bit_functions;

        // // Create all of the output_bit_example_sets
        // for (int i = 0; i < a_raw_examples[0].m_output.size(); i++)
        // {
        //     // Do work asynchronously so as to reduce compute time.
        //     l_async_output_bit_functions.push_back(
        //         std::async(std::launch::async, [&, i]{ return generalize(a_cache, partitioned_example_set(a_raw_examples, i)); }));
        // }

        // std::vector<sum_of_products> l_output_bit_functions;
        
        // // Resolve all asynchronous outputs.
        // for (int i = 0; i < l_async_output_bit_functions.size(); i++)
        //     l_output_bit_functions.push_back(l_async_output_bit_functions[i].get());

        std::vector<sum_of_products> l_output_bit_functions;

        for (int i = 0; i < a_raw_examples[0].m_output.size(); i++)
        {
            l_output_bit_functions.push_back(generalize(a_cache, partitioned_example_set(a_raw_examples, i)));
        }

        return sum_of_products_string(l_output_bit_functions);

    }

}

#endif
