let component = ReasonReact.statelessComponent("Episode");

let make = (~title, _children) => {
  ...component,
  render: (_self) => <div> (ReasonReact.stringToElement(title)) </div>
};