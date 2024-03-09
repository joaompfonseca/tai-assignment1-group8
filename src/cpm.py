from random import choice
from math import log2

# Define constants
THRESHOLD = 0.2
SMOOTHING_FACTOR = 1.0
WINDOW_SIZE = 3


# --- 1st pass through the file (stream and alphabet in memory) ---
stream = "GATGAAGATTAGAT"
alphabet = "AGT"

counts = {}
for char in stream:
    counts[char] = counts.get(char, 0) + 1
# ----------------
# se sequence_map["AAG"] = [3 7 12] e estava a usar o 3, posso passar a usar o 7 ou o 12
# se sequence_map["AAG"] = [3] e estava a usar o 3, usar a freq do símbolo que saiu


# Future work:
# tirar aquilo que ja foi processado (decrementar counts)
# arrancar novos copy models com as últimas posições (limite de copy models ativos para uma mesma sequência)

hits = 0
misses = 0
estimated_number_of_bits = 0
sequence_map = {}
active_copy_models = {}
ptr = 0
stream_size = len(stream)

class CopyModel:
    def __init__(self, reference=None):
        self.reference = reference
        self.anchors = []
        if self.reference: self.anchors.append(reference)
        self.hits = 0
        self.misses = 0

    def add_anchor(self, position):
        if self.reference:
            self.anchors.append(position)
        else:
            self.reference = position
            self.anchors = [position]

    def update_reference(self, remove_current=True, criteria="better"):
        if self.reference and remove_current:
            self.anchors.remove(self.reference)
        if self.anchors:
            if criteria == "last":
                self.reference = self.anchors[-1]
            elif criteria == "first":
                self.reference = self.anchors[0]
            elif criteria == "better":
                self.reference = max(self.anchors, key=lambda x: self.hits / (self.hits + self.misses))
            elif criteria == "random":
                self.reference = choice(self.anchors)
        else:
            self.reference = None

    def hit(self):
        self.hits += 1

    def miss(self):
        self.misses += 1

    def __str__(self):
        return f"CopyModel({self.reference}, {self.hits}, {self.misses})"

while ptr < stream_size:
        
    sequence = alphabet[0] * (WINDOW_SIZE - ptr) + stream[:ptr] if ptr - WINDOW_SIZE < 0 else stream[ptr - WINDOW_SIZE:ptr]

    if not sequence_map or sequence in sequence_map:

        if sequence_map:
            copy_model = sequence_map[sequence]
            pred_char = stream[copy_model.reference]
            
        else:
            copy_model = CopyModel()
            sequence_map[sequence] = copy_model
            pred_char = alphabet[0]
            
        hits, misses = copy_model.hits, copy_model.misses # COMMENT THIS LINE IF YOU WANT TO USE THE GLOBAL HITS AND MISSES
        actual_char = stream[ptr]
        symbol_prob = (hits + SMOOTHING_FACTOR) / (hits + misses + 2 * SMOOTHING_FACTOR)

        if pred_char == actual_char:
            hits += 1
            estimated_number_of_bits += -log2(symbol_prob)
        else:
            misses += 1
            # we have to subract the predicted char count from the stream length, since we failed the prediction
            relative_freq = counts[actual_char] / (stream_size - counts[pred_char])
            estimated_number_of_bits += -log2((1 - symbol_prob) * relative_freq)

        if sequence_map and hits / (hits + misses) < THRESHOLD:
            # removing the current reference anchor is optional
            copy_model.update_reference(remove_current=True)

        copy_model.add_anchor(ptr)

    else:
        # use relative frequency of the actual char
        estimated_number_of_bits += - log2(counts[actual_char] / stream_size)
        sequence_map[sequence] = CopyModel(ptr)

    ptr += 1

    print(f"Estimated: {estimated_number_of_bits}")

# TODO: add overhead + decrement already processed counts + trigger new copy models
