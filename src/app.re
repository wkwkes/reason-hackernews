[%bs.raw {|require('./app.css')|}];

open Constants;

open Utils;

type state = {
  results: Hashtbl.t(string, (int, list(data))),
  searchTerm: string,
  isLoading: bool
};

type action =
  | Click(Hashtbl.t(string, (int, list(data))))
  | Change(string)
  | Update(Hashtbl.t(string, (int, list(data))))
  | Submit(string)
  | UpdateLoad(bool)
  | Nope;

let component = ReasonReact.reducerComponent("App");

let onDismiss = (results, searchTerm, id, _evt) => {
  let (page, list) = Hashtbl.find(results, searchTerm);
  Hashtbl.add(results, searchTerm, (page, List.filter((item) => item.objectID != id, list)));
  Click(results)
};

let fetchSearchTopStories = (reducer, isloading, results, searchTerm, load) => {
  let _ = isloading(true);
  let url = path_base ++ path_search ++ "?" ++ param_search ++ searchTerm ++ "&" ++ page_param;
  let fetch = (url, res, page) =>
    Js.Promise.(
      Bs_fetch.(
        ReadJson.(
          fetch(url)
          |> then_(Response.text)
          |> then_((jsonText) => resolve(parseDataResponseJson(Js.Json.parseExn(jsonText))))
          |> then_(
               (data) => {
                 Hashtbl.add(results, searchTerm, (page, res @ data));
                 reducer(results);
                 isloading(false);
                 resolve()
               }
             )
          |> catch((_) => resolve())
          |> ignore
        )
      )
    );
  switch (Hashtbl.find(results, searchTerm)) {
  | (page, res) =>
    if (load) {
      fetch(url ++ string_of_int(page + 1), res, page + 1)
    } else {
      reducer(results)
    }
  | exception Not_found => fetch(url ++ "0", [], 0)
  };
};

let onSearchChange = (evt) => {
  let vl = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(evt))##value;
  Change(vl)
};

let onSearchSubmit = (reducer, isloading, results, evt) => {
  ReactEventRe.Form.preventDefault(evt);
  let vl = ((ReactDOMRe.domElementToObj(ReactEventRe.Form.target(evt))##children)[0])##value;
  /* let vl = ((ReactDOMRe.domElementToObj(ReactEventRe.Form.target(evt))##children)[0])##value; */    
  fetchSearchTopStories(reducer, isloading, results, vl, false);
  Nope
};

let onLoadMore = (reducer, isloading, results, searchTerm, _) => {
  fetchSearchTopStories(reducer, isloading, results, searchTerm, true);
  Nope
};

let make = (_children) => {
  ...component,
  initialState: () => {results: Hashtbl.create(100), searchTerm: default_query, isLoading: true},
  reducer: (action, state) =>
    switch action {
    | Click(res) => ReasonReact.Update({...state, results: res})
    | Change(inp) => ReasonReact.Update({...state, searchTerm: inp})
    | Update(res) => ReasonReact.Update({...state, results: res})
    | Submit(st) => ReasonReact.Update({...state, searchTerm: st})
    | UpdateLoad(b) => ReasonReact.Update({...state, isLoading: b})
    | Nope => ReasonReact.NoUpdate
    },
  didMount: (self) => {
    fetchSearchTopStories(
      self.reduce((data) => Update(data)),
      self.reduce(b => UpdateLoad(b)),
      self.state.results,
      self.state.searchTerm,
      false
    );
    ReasonReact.NoUpdate;
  },
  render: (self) => {
    <div className="page">
      <div className="interactions">
        <Search
          value=self.state.searchTerm
          onChange=(self.reduce(onSearchChange))
          onSubmit=(
            self.reduce(onSearchSubmit(self.reduce((data) => Update(data)), self.reduce((b) => UpdateLoad(b)), self.state.results))
          )>
          (s2e("Search "))
        </Search>
      </div>
      <Table
        list=(
               try Hashtbl.(snd(find(self.state.results, self.state.searchTerm))) {
               | Not_found => []
               }
             )
        onDissmiss=((id) => self.reduce(onDismiss(self.state.results, self.state.searchTerm, id)))
      />
      <div className="interactions">
          <WithLoad
            onClick=(
              self.reduce(
                onLoadMore(
                  self.reduce((data) => Update(data)),
                  self.reduce((b) => UpdateLoad(b)),
                  self.state.results,
                  self.state.searchTerm
                )
              )
            )
            isLoading=self.state.isLoading>
            (s2e("More"))
          </WithLoad>
        </div>
    </div>
              }
};