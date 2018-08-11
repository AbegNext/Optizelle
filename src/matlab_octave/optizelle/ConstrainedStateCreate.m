% Creates a constrained state
function self=ConstrainedStateCreate(X,Y,Z,x,y,z)
    % Check our arguments
    checkVectorSpace('X',X);
    checkVectorSpace('Y',Y);
    checkVectorSpace('Z',Z);

    % Create the state
    self=ConstrainedStateCreate_(X,Y,Z,x,y,z);
