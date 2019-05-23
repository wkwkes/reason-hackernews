open Utils;

let parseDataJson = (json: Js.Json.t): data =>
  Json.Decode.{
    title: field("title", string, json),
    url: field("url", string, json),
    author: field("author", string, json),
    num_comments: field("num_comments", int, json),
    points: field("points", int, json),
    objectID: field("objectID", string, json),
  };

let parseDataResponseJson = json =>
  Json.Decode.field("hits", Json.Decode.list(parseDataJson), json);