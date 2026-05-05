
import random

def monty_hall_simulation(num_trials=10000):
    win_switch = 0
    win_stay = 0
    
    for _ in range(num_trials):
        # Doors: 0 = car, 1 and 2 = goats (sheep)
        car_door = random.randint(0, 2)
        player_choice = random.randint(0, 2)
        
        # Host opens a goat door (not the player's choice or the car)
        possible_open = [i for i in range(3) if i != player_choice and i != car_door]
        host_opens = random.choice(possible_open)
        
        # Switch to the remaining door
        switch_choice = [i for i in range(3) if i != player_choice and i != host_opens][0]
        
        # Check wins
        if player_choice == car_door:
            win_stay += 1
        if switch_choice == car_door:
            win_switch += 1
    
    prob_stay = win_stay / num_trials
    prob_switch = win_switch / num_trials
    
    print(f"Probability of winning by staying: {prob_stay:.3f}")
    print(f"Probability of winning by switching: {prob_switch:.3f}")
    print("The 'sheep will not escape' (you win the car) probability is higher when switching.")

monty_hall_simulation()
