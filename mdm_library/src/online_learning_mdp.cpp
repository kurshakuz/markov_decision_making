/**\file online_learning_mdp.cpp
 *
 * Author:
 * Pedro Resende <pt.resende@gmail.com>
 *
 * Markov Decision Making is a ROS library for robot decision-making based on MDPs.
 * Copyright (C) 2014 Instituto Superior Tecnico, Instituto de Sistemas e Robotica
 *
 * This file is part of Markov Decision Making.
 *
 * Markov Decision Making is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Markov Decision Making is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <mdm_library/online_learning_mdp.h>


using namespace std;
using namespace mdm_library;

#ifdef HAVE_MADP

OnlineLearningMDP::
OnlineLearningMDP ( float gamma,
                    ALPHA_TYPE alpha_type,
                    float alpha_value,
                    EPSILON_TYPE epsilon_type,
                    float epsilon,
                    uint32_t policy_update_frequency,
                    const string& policy_file_path,
                    const string& problem_file_path,
                    const ControlLayerBase::CONTROLLER_STATUS initial_status ) :
    controller_ ( policy_file_path, problem_file_path, initial_status ),
    gamma_ ( gamma ),
    alpha_type_ ( alpha_type ),
    alpha_ ( alpha ),
    epsilon_type_ ( epsilon_type ),
    epsilon_ ( epsilon ),
    policy_update_frequency_ ( policy_update_frequency ),
    curr_decision_ep_ ( 0 ),
    state_sub_ ( nh_.subscribe ( "state", 1, &OnlineLearningMDP::stateSymbolCallback, this ) ),
    action_sub_ ( nh_.subscribe ( "action", 1, &OnlineLearningMDP::actionSymbolCallback, this ) ),
    reward_sub_ ( nh_.subscribe ( "reward", 1, &OnlineLearningMDP::rewardSymbolCallback, this ) )
{
    initializeQValues ();
}

#endif

OnlineLearningMDP::
OnlineLearningMDP ( float gamma,
                    ALPHA_TYPE alpha_type,
                    EPSILON_TYPE epsilon_type,
                    uint32_t policy_update_frequency,
                    const string& policy_file_path,
                    const ControlLayerBase::CONTROLLER_STATUS initial_status ) :
    controller_ ( policy_file_path, epsilon_type, epsilon_value, initial_status ),
    gamma_ ( gamma ),
    alpha_type_ ( alpha_type ),
    alpha_ ( -1 ),
    epsilon_type_ ( epsilon_type ),
    epsilon_ ( -1 ),
    policy_update_frequency_ ( policy_update_frequency ),
    curr_decision_ep_ ( 0 ),
    state_sub_ ( nh_.subscribe ( "state", 1, &OnlineLearningMDP::stateSymbolCallback, this ) ),
    action_sub_ ( nh_.subscribe ( "action", 1, &OnlineLearningMDP::actionSymbolCallback, this ) ),
    reward_sub_ ( nh_.subscribe ( "reward", 1, &OnlineLearningMDP::rewardSymbolCallback, this ) )
{
    initializeQValues ();
}



OnlineLearningMDP::
OnlineLearningMDP ( float gamma,
                    float alpha_value,
                    EPSILON_TYPE epsilon_type,
                    uint32_t policy_update_frequency,
                    const string& policy_file_path,
                    const ControlLayerBase::CONTROLLER_STATUS initial_status ) :
    controller_ ( policy_file_path, initial_status ),
    gamma_ ( gamma ),
    alpha_type_ ( alpha_type ),
    epsilon_type_ ( epsilon_type ),
    policy_update_frequency_ ( policy_update_frequency ),
    curr_decision_ep_ ( 0 ),
    state_sub_ ( nh_.subscribe ( "state", 1, &OnlineLearningMDP::stateSymbolCallback, this ) ),
    action_sub_ ( nh_.subscribe ( "action", 1, &OnlineLearningMDP::actionSymbolCallback, this ) ),
    reward_sub_ ( nh_.subscribe ( "reward", 1, &OnlineLearningMDP::rewardSymbolCallback, this ) )
{
    initializeQValues ();
}



void
OnlineLearningMDP::
initializeQValues ()
{
    num_states_ = controller_.getNumberOfStates ();
    num_actions_ = controller_.getNumberOfActions ();
    
    Matrix q_values_ ( num_states_, num_actions_ );
    
    // Initialize the Q values as 0
    for ( unsigned i = 0; i < q_values_.size1(); i++ )
        for ( unsigned j = 0; j < q_values_.size2(); j++ )
            q_values_ ( i, j ) = 0;
}
