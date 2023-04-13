# dspd_project3

Hungry app, desktop application, is created which allows users to order food from restaurants(available in restaurant.csv file) and get them deliveried

To write this application, i used AVL trees as data structure.

first, all 3 AVL trees(restraunt,agent,user) are created by reading data from their respective files.

then orders from users are taken and these orders are also arranged in global_order_tree(avl). to assign a delivery agent for a particular order,we keep a pointer to agent node in structure of global_order_node of global_order_tree, so that by inorder traversal we will go to first order(by order id as key) and we wil assign to a agent(by inorder traversal, least agent id(agent who is first in line)) by agent pointer in structure in global_order_node and we will set agent status to 1(indicates agent is not free),after delivering order we will set agent status to 0( indicates agent is free and ready for another order to get assigned).

we will perform all functions which are in question, like search operations, cancel order,delivery order...
