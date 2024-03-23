import matplotlib.pyplot as plt

# Read data from file
data_file = 'Tests/TestCircuit1/outputSimulation1.sim'
data = []
with open(data_file, 'r') as file:
    for line in file:
        timestamp, wire, state = line.split()
        data.append((int(timestamp) / 100, wire, int(state)))

# Extract unique wires and their initial states
wires_initial_states = {wire: 0 for _, wire, _ in data}

# Initialize wire states
wire_states = {wire: [initial_state] for wire, initial_state in wires_initial_states.items()}

# Update wire states based on simulation output data
for timestamp, wire, state in data:
    # Extend the wire state list up to the specified timestamp
    for w in wire_states:
        if len(wire_states[w]) < timestamp:
            wire_states[w].extend([wire_states[w][-1]] * (int(timestamp) - len(wire_states[w])))
    # Change the state of the wire at the specified timestamp
    wire_states[wire].append(state)

# Calculate the maximum length of wire states
max_length = max(len(states) for states in wire_states.values())

# Set the desired maximum timestamp (adjust as needed)
desired_max_timestamp = 15  # Adjust based on your data

# Create a single plot for all input wires
fig, ax = plt.subplots(len(wire_states), 1, figsize=(10, 8), sharex=True)

# Plot each input wire on the same screen
for i, (wire, states) in enumerate(wire_states.items()):
    # Adjust data slicing based on desired maximum timestamp
    plot_data = states[:desired_max_timestamp] + [states[-1]] * (desired_max_timestamp - len(states))

    ax[i].step(range(0, desired_max_timestamp * 100, 100), plot_data, where='post')
    ax[i].set_title(f'Wire {wire} State Change Over Time')
    ax[i].set_xlim(0, desired_max_timestamp * 100)  # Set x-axis limits
    ax[i].set_xticks(range(0, (desired_max_timestamp + 1) * 100, 100))  # Set x-axis tick positions
    ax[i].set_ylabel('Wire State')
    ax[i].set_xlabel('Time (ps)')  # Set x-axis label

    # Set y-axis limits for wires W1 and W3
    if wire in ['W1', 'W3']:
        ax[i].set_ylim(0, 1)

# Set x-axis label for the last subplot
ax[-1].set_xlabel('Time (ps)')

plt.tight_layout()
plt.show()
