type data = {
  title: string,
  url: string,
  author: string,
  num_comments: int,
  points: int,
  objectID: string,
};

let s2e = React.string;

let i2e = x => s2e(string_of_int(x));

let l2e = x => React.array(Array.of_list(x));