
open Utils;

let component = ReasonReact.statelessComponent("Search");
let make = (~value, ~onChange, ~onSubmit, _children) => {
  ...component,
  render: (_) =>
    <form onSubmit>
      <input _type="text" value onChange />
      <button _type="submit"> (s2e("Search")) </button>
    </form>
};