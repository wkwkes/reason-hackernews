[%bs.raw {|require('./app.css')|}];

open Constants;

open Utils;

type state = {
  results: Hashtbl.t(string, (int, list(data))),
  searchTerm: string,
  isLoading: bool,
};

type action =
  | Click(Hashtbl.t(string, (int, list(data))))
  | Change(string)
  | Update(Hashtbl.t(string, (int, list(data))))
  | Submit(string)
  | UpdateLoad(bool)
  | Nope;

// let component = ReasonReact.reducerComponent("App");

let onDismiss = (results, searchTerm, id, _evt) => {
  let (page, list) = Hashtbl.find(results, searchTerm);
  Hashtbl.add(
    results,
    searchTerm,
    (page, List.filter(item => item.objectID != id, list)),
  );
  Click(results);
};

let fetchSearchTopStories = (reducer, isloading, results, searchTerm, load) => {
  let _ = isloading(true);
  let url =
    path_base
    ++ path_search
    ++ "?"
    ++ param_search
    ++ searchTerm
    ++ "&"
    ++ page_param;
  let fetch = (url, res, page) =>
    Js.Promise.(
      Bs_fetch.(
        ReadJson.(
          fetch(url)
          |> then_(Response.text)
          |> then_(jsonText =>
               resolve(parseDataResponseJson(Js.Json.parseExn(jsonText)))
             )
          |> then_(data => {
               Hashtbl.add(results, searchTerm, (page, res @ data));
               reducer(results);
               isloading(false);
               resolve();
             })
          |> catch(_ => resolve())
          |> ignore
        )
      )
    );
  switch (Hashtbl.find(results, searchTerm)) {
  | (page, res) =>
    if (load) {
      fetch(url ++ string_of_int(page + 1), res, page + 1);
    } else {
      reducer(results);
    }
  | exception Not_found => fetch(url ++ "0", [], 0)
  };
};

let onSearchChange = evt => {
  let vl = ReactEvent.Form.target(evt)##value;
  Change(vl);
};

let onSearchSubmit = (reducer, isloading, results, evt) => {
  /* ReactEventRe.Form.preventDefault(evt); */
  evt->ReactEvent.Form.preventDefault;
  let vl = ReactEvent.Form.target(evt)##children[0]##value;
  /* let vl = ((ReactDOMRe.domElementToObj(ReactEventRe.Form.target(evt))##children)[0])##value; */
  fetchSearchTopStories(reducer, isloading, results, vl, false);
  Nope;
};

let onLoadMore = (reducer, isloading, results, searchTerm, _) => {
  fetchSearchTopStories(reducer, isloading, results, searchTerm, true);
  Nope;
};

[@react.component]
let make = () => {
  // ...component,
  // initialState: () => {results: Hashtbl.create(100), searchTerm: default_query, isLoading: true},
  let reducer = (state, action) =>
    switch (action) {
    | Click(res) => {...state, results: res}
    | Change(inp) => {...state, searchTerm: inp}
    | Update(res) => {...state, results: res}
    | Submit(st) => {...state, searchTerm: st}
    | UpdateLoad(b) => {...state, isLoading: b}
    | Nope => state
    };
  let (state, reduce) =
    React.useReducer(
      reducer,
      {
        results: Hashtbl.create(100),
        searchTerm: default_query,
        isLoading: true,
      },
    );
  React.useEffect0(() => {
    Printf.printf("print");
    fetchSearchTopStories(
      data => reduce(Update(data)),
      b => reduce(UpdateLoad(b)),
      state.results,
      state.searchTerm,
      false,
    );
    Some(() => ());
  });
  <div className="page">
    <div className="interactions">
      <Search
        value={state.searchTerm}
        onChange={evt => reduce(onSearchChange(evt))}
        onSubmit={evt =>
          reduce(
            onSearchSubmit(
              data => reduce(Update(data)),
              b => reduce(UpdateLoad(b)),
              state.results,
              evt,
            ),
          )
        }
      />
    </div>
    /* { (s2e("Search ")) } */
    /* </Search> */
    <Table
      list={
             try (Hashtbl.(snd(find(state.results, state.searchTerm)))) {
             | Not_found => []
             }
           }
      onDissmiss={(id, evt) =>
        reduce(onDismiss(state.results, state.searchTerm, id, evt))
      }
    />
    <div className="interactions">
      <WithLoad
        onClick={evt =>
          reduce(
            onLoadMore(
              data => reduce(Update(data)),
              b => reduce(UpdateLoad(b)),
              state.results,
              state.searchTerm,
              evt,
            ),
          )
        }
        isLoading={state.isLoading}>
        {s2e("More")}
      </WithLoad>
    </div>
  </div>;
};

/* let make = () => Obj.magic(make()) */