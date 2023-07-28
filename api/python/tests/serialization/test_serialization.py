from dlplan.serialization import Data, serialize, deserialize


def test_serialization_empty():
    out_data = Data()
    text = serialize(out_data)
    in_data = deserialize(text)
    assert in_data.state_spaces == []
