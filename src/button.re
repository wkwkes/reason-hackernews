
let component = ReasonReact.statelessComponent("Button");
let make = (~onClick, ~className="", children) => {
  ...component,
  render: (_) => <button onClick className _type="button"> children[0] </button>
};