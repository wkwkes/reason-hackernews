open Utils;

// let component = ReasonReact.statelessComponent("Search");

[@react.component]
let make = (~value, ~onChange, ~onSubmit) =>
  <form onSubmit>
    <input type_="text" value onChange />
    <button type_="submit"> {s2e("Search")} </button>
  </form>;