#ifndef EXAMPLES_H
#define EXAMPLES_H

#include <vector>
#include <stddef.h>

namespace boolean_ai
{
    /// @brief This struct has a bit string as input and output.
    struct raw_example
    {
        std::vector<bool> m_input;
        std::vector<bool> m_output;
    };

    /// @brief This is a bit-string which does not satisfy the function which it is being inputted into.
    typedef std::vector<bool> unsatisfying_input;

    /// @brief This is a bit-string which does satisfy the function which it is being inputted into.
    typedef std::vector<bool> satisfying_input;

    /// @brief This struct has a set of unsatisfying inputs, and a set of satisfying inputs.
    ///        These sets will be used for generalization later.
    struct partitioned_example_set
    {
        std::vector<unsatisfying_input*> m_unsatisfying_inputs;
        std::vector<satisfying_input*> m_satisfying_inputs;
        partitioned_example_set(
            const std::vector<raw_example>& a_raw_examples,
            const size_t& a_index
        )
        {
            for (const raw_example& l_raw_example : a_raw_examples)
            {
                // Use pointers to preserve memory.
                if (l_raw_example.m_output[a_index])
                    m_satisfying_inputs.push_back((satisfying_input*)&l_raw_example.m_input);
                else
                    m_unsatisfying_inputs.push_back((unsatisfying_input*)&l_raw_example.m_input);
            }
        }
    };

}

#endif
