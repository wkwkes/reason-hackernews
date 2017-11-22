type data = {
    title: string,
    url: string,
    author: string,
    num_comments: int,
    points: int,
    objectID: string
  };
  
  let s2e = ReasonReact.stringToElement;
  
  let i2e = (x) => s2e(string_of_int(x));
  
  let l2e = (x) => ReasonReact.arrayToElement(Array.of_list(x));