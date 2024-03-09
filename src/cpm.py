from random import choice
from math import log2


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

THRESHOLD = 0.2
SMOOTHING_FACTOR = 1.0
WINDOW_SIZE = 3


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

    def update_reference(self, remove_current=True, criteria="last"):
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

class CopyModelRunner:
    def __init__(self, stream, threshold, smoothing_factor, window_size, global_metrics=False):
        self.stream = stream
        self.stream_size = len(stream)
        self.threshold = threshold
        self.smoothing_factor = smoothing_factor
        self.window_size = window_size
        self.global_metrics = global_metrics
        self.sequence_map = {}
        self.estimated_number_of_bits = 0
        self.ptr = 0

        if self.global_metrics:
            self.hits = 0
            self.misses = 0

    def has_next(self):
        return self.ptr < self.stream_size

    def run_step(self):
        sequence = alphabet[0] * (WINDOW_SIZE - self.ptr) + stream[:self.ptr] if self.ptr - WINDOW_SIZE < 0 else stream[self.ptr - WINDOW_SIZE:self.ptr]
        actual_char = stream[self.ptr] # to be accessed after prediction

        if not self.sequence_map or sequence in self.sequence_map:

            if self.sequence_map:
                copy_model = self.sequence_map[sequence]
                pred_char = stream[copy_model.reference]
                
            else:
                copy_model = CopyModel()
                self.sequence_map[sequence] = copy_model
                pred_char = alphabet[0]

            print(f"Predicted: {pred_char}, Actual: {actual_char}")

            if not self.global_metrics: 
                self.hits, self.misses = copy_model.hits, copy_model.misses

            symbol_prob = (self.hits + self.smoothing_factor) / (self.hits + self.misses + 2 * self.smoothing_factor)

            if pred_char == actual_char:
                self.hits += 1
                self.estimated_number_of_bits += -log2(symbol_prob)
                print("Hit")
            else:
                self.misses += 1
                # we have to subract the predicted char count from the stream length, since we failed the prediction
                relative_freq = counts[actual_char] / (self.stream_size - counts[pred_char])
                self.estimated_number_of_bits += -log2((1 - symbol_prob) * relative_freq)
                print("Miss")

            if self.sequence_map and self.hits / (self.hits + self.misses) < self.threshold:
                # removing the current reference anchor is optional
                copy_model.update_reference(remove_current=True)

            copy_model.add_anchor(self.ptr)

        else:
            print("Sequence not found")
            # use relative frequency of the actual char
            self.estimated_number_of_bits += - log2(counts[actual_char] / self.stream_size)
            self.sequence_map[sequence] = CopyModel(self.ptr)

        self.ptr += 1

        print(f"Estimated: {self.estimated_number_of_bits}")


runner = CopyModelRunner(stream, THRESHOLD, SMOOTHING_FACTOR, WINDOW_SIZE, global_metrics=True)
while runner.ptr < runner.stream_size:
    runner.run_step()

# TODO: add overhead + decrement already processed counts + trigger new copy models
