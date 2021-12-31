import logging

from collections import defaultdict

from transition_system import TransitionSystem, Object, Predicate, Atom, State
from utils import read_file, filename_core


class TransitionSystemReader():
    def read_from_file(self, filename):
        """ Reads a transition system from file. """
        states, goals, forward_graph, deadends, atoms, static_atoms, predicates, objects = self._read_single_sample_file(filename)
        # if no goals are found we can ignore this transition system and return None to indicate this
        if goals:
            return TransitionSystem(states, forward_graph, self._compute_inverse_graph(forward_graph), goals, deadends, atoms, static_atoms, predicates, objects, states[0])
        else:
            return None

    def _normalize_atom_name(self, name):
        tmp = name.replace('()', '').replace(')', '').replace('(', ',')
        if "=" in tmp:  # We have a functional atom
            tmp = tmp.replace("=", ',')

        return tmp.split(',')

    def _parse_atom(self, atom_name, atom_name_to_idx, predicate_name_to_idx, object_name_to_idx, atoms, predicates, objects, is_static):
        normalized_atom_name = self._normalize_atom_name(atom_name)
        predicate_name = normalized_atom_name[0]
        # disallow functional atoms
        assert predicate_name != "="
        object_names = normalized_atom_name[1:]
        atom_idx = atom_name_to_idx.get(atom_name, None)
        if atom_idx is None:
            predicate_idx = predicate_name_to_idx.get(predicate_name, None)
            if predicate_idx is None:
                predicate_name_to_idx[predicate_name] = len(predicates)
                predicate = Predicate(len(predicates), predicate_name, len(object_names))
                predicates.append(predicate)
            else:
                predicate = predicates[predicate_idx]
            atom_objects = []
            for object_name in object_names:
                object_idx = object_name_to_idx.get(object_name, None)
                if object_idx is None:
                    object_name_to_idx[object_name] = len(objects)
                    object = Object(len(objects), object_name)
                    objects.append(object)
                else:
                    object = objects[object_idx]
                atom_objects.append(object)

            atom_name_to_idx[atom_name] = len(atoms)
            atom = Atom(len(atoms), atom_name, predicate, atom_objects, is_static)
            atoms.append(atom)
        else:
            atom = atoms[atom_idx]
        return atom

    def _compute_inverse_graph(self, graph):
        """ Compute the inverse of a graph. """
        inverse_graph = defaultdict(set)
        for source, targets in graph.items():
            for t in targets:
                inverse_graph[t].add(source)
        return inverse_graph

    def _read_single_sample_file(self, filename):
        states = dict()
        transitions = defaultdict(set)
        transitions_inv = defaultdict(set)
        goals = set()
        deadends = set()

        objects = []
        object_name_to_idx = dict()

        predicates = []
        predicate_name_to_idx = dict()

        atoms = []
        atom_name_to_idx = dict()

        static_atoms = []
        static_atom_name_to_idx = dict()

        # Parse the header
        nlines = 0  # The number of useful lines processed
        for line in read_file(filename):
            nlines += 1
            if line.startswith('(S)'):
                atom_name = line[4:]
                self._parse_atom(atom_name, static_atom_name_to_idx, predicate_name_to_idx, object_name_to_idx, static_atoms, predicates, objects, True)
            elif line.startswith('(V)'):
                atom_name = line[4:]
                atom = self._parse_atom(atom_name, atom_name_to_idx, predicate_name_to_idx, object_name_to_idx, atoms, predicates, objects, False)
            elif line.startswith('Unable to fully explore state space with max_expansions:'):
                return None, None, None, None, None, None, None, None

        #print()
        #print("Variable atoms:")
        #print("\n".join([atom.name for atom in atoms]))
        #print()
        #print("Static atoms:")
        #print("\n".join([atom.name for atom in static_atoms]))
        #print()

        # Parse the body
        for line in read_file(filename):
            if line.startswith('(E)'):  # An edge, with format "(E) 5 12"
                pid, cid = (int(x) for x in line[4:].split(' '))
                transitions[pid].add(cid)
                transitions_inv[cid].add(pid)
                nlines += 1

            elif line.startswith('(N)'):  # A node
                # Format "(N) <id> <type> <space-separated-atom-list>", e.g.:
                # (N) 12 G
                elems = line[4:].split(' ')
                sid = int(elems[0])
                if elems[1] == 'G':  # The state is a goal state
                    goals.add(sid)
                if elems[1] == 'D':  # The state is a dead-end
                    deadends.add(sid)

                state_atoms = []
                for atom_name in elems[2:]:
                    atom = self._parse_atom(atom_name, atom_name_to_idx, predicate_name_to_idx, object_name_to_idx, atoms, predicates, objects, False)
                    state_atoms.append(atom)
                states[sid] = State(sid, state_atoms)
        # Make sure all edge IDs have actually been declared as a state
        state_idxs = list(states.keys())
        for src in transitions:
            assert src in state_idxs and all(dst in state_idxs for dst in transitions[src])

        # Make sure that the number of outgoing and incoming edges coincide
        num_tx = sum(len(t) for t in transitions.values())
        assert num_tx == sum(len(t) for t in transitions_inv.values())

        logging.info('%s: #lines-raw-file=%d, #states=%d, #transitions=%d' %
                    (filename_core(filename), nlines, len(states), num_tx))

        return states, goals, transitions, deadends, atoms, static_atoms, predicates, objects
