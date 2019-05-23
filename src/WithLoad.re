open Utils;

[@react.component]
let make = (~onClick, ~isLoading, ~children) =>
  if (isLoading) {
    <div> {s2e("Loading...")} </div>;
  } else {
    <Button onClick> children </Button>;
  };