open Utils;

let component = ReasonReact.statelessComponent("WithLoad");

let make = (~onClick, ~isLoading, children) => {
  ...component,
  render: (_) => {
    (isLoading) ? <div> (s2e("Loading...")) </div> : <Button onClick> children[0] </Button>
  }
};