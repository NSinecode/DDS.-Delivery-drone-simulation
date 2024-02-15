from Agent import Position, target, NewTarger, Position, Vel, Ax, Angel, isrunning
import Player
import keyboard

a=0
b=0
class Direction(Enum):
    RIGHT = 1
    LEFT = 2
    UP = 3
    DOWN = 4

class DroneAI:
    
    def play_step(self, action):
        a+=1
        self._move(action)
        
        reward = 0
        game_over = False
        
        if a>=100:
            game_over = True
            reward = -10
            return reward, game_over, self.score
            
        if Agent.Position == Agent.target:
            self.score += 1
            reward = 10
            b+=1
            Agent.NewTarger(b)
            return reward, game_over, self.score

    def _move(self, action):
        # [straight, right, left]
        
        clock_wise = [Direction.RIGHT, Direction.DOWN, Direction.LEFT, Direction.UP]
        idx = clock_wise.index(self.direction)
        
        if np.array_equal(action, [1, 0, 0]):
            new_dir = clock_wise[idx] # no change
        elif np.array_equal(action, [0, 1, 0]):
            next_idx = (idx + 1) % 4
            new_dir = clock_wise[next_idx] # r - d - l - u (right turn)
        else: # [0, 0, 1]
            next_idx = (idx - 1) % 4
            new_dir = clock_wise[next_idx] # r - u - l - d (left turn)
            
        self.direction = new_dir
            
        if self.direction == Direction.RIGHT:
            Player.RIGHT(Agent.Angel)
            keyboard.press("6")
        elif self.direction == Direction.LEFT:
            Player.LEFT(Agent.Angel)
            keyboard.press("4")
        elif self.direction == Direction.UP:
            Player.UP(Agent.Position, Agent.Vel, Agent.Ax, Agent.Angle, Agent.isrunning)
            keyboard.press("5")
